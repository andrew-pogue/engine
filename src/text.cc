#include <allegro5/allegro_color.h>
#include <cassert>
#include <cmath>
#include "math.hh"
#include "text.hh"

///////////////////////////////////////////////////////////////////////////////
/// LOCAL API

namespace {

void draw_ustr_with_effect(
    float x, float y,
    const ALLEGRO_FONT *font, ALLEGRO_COLOR color, const ALLEGRO_USTR *ustr,
    TextEffect effect
) {
    static auto draw_char =
        [&effect, font](int ch, float x, float y, ALLEGRO_COLOR color) {
            // changes made by effect are scoped to this lambda
            if (effect) effect(ch, x, y, color);
            al_draw_glyph(font, color, x, y, ch);
        };

    int pos = al_ustr_offset(ustr, 0),
        prev_ch = al_ustr_get(ustr, pos);
    if (prev_ch >= 0) draw_char(prev_ch, x, y, color);

    while (al_ustr_next(ustr, &pos)) {
        int ch = al_ustr_get(ustr, pos);
        if (ch < 0) continue;
        x += al_get_glyph_advance(font, prev_ch, ch);
        draw_char(ch, x, y, color);
        prev_ch = ch;
    }
}

struct MultilineData {
    float x, y, height;
    const ALLEGRO_FONT *font;
    const ALLEGRO_COLOR color;
    const AlignX align;
    const float spacing;
    const TextEffect effect;
    float advance = 0.f;
};

bool draw_fancy_text_helper(int ln, const ALLEGRO_USTR *ustr, void *extra) {
    MultilineData &data = *(MultilineData *)extra;

    float y = data.y + data.advance;
    data.advance += al_get_font_line_height(data.font) + data.spacing;
    if (y < data.y) return true;
    if (y > data.y + data.height) return false;

    float width = al_get_ustr_width(data.font, ustr);
    float x = data.x
        - float(data.align == AlignX::RIGHT) * width
        - float(data.align == AlignX::CENTER) * width / 2.f;
    
    ALLEGRO_USTR_INFO info;
    if (data.effect) 
        draw_ustr_with_effect(x, y, data.font, data.color,
            al_ref_ustr(&info, ustr, 0, al_ustr_size(ustr)), data.effect);
    else al_draw_ustr(data.font, data.color, x, y, 0, al_ref_ustr(&info, ustr, 0, al_ustr_size(ustr)));
    return true;
}

bool get_ustr_line_count_helper(int ln, const ALLEGRO_USTR *str, void *count) {
    *(int *)count = ln + 1;
    return true;
}

bool get_cstr_line_count_helper(int ln, const char *str, int size, void *count) {
    *(int *)count = ln + 1;
    return true;
}

} // namespace

///////////////////////////////////////////////////////////////////////////////
/// GLOBAL API

void draw_text(
    Rectangle bounds, AlignX align_x, AlignY align_y,
    const ALLEGRO_FONT *font, ALLEGRO_COLOR color, const char *cstr,
    float spacing, TextEffect effect
) {
    assert(font); assert(cstr);
    if (!font || !cstr) return;

    ALLEGRO_USTR_INFO info;
    draw_text(bounds, align_x, align_y, font, color, al_ref_cstr(&info, cstr), spacing, effect);
}

void draw_text(
    Rectangle bounds, AlignX align_x, AlignY align_y,
    const ALLEGRO_FONT *font, ALLEGRO_COLOR color, const ALLEGRO_USTR *ustr,
    float spacing, TextEffect effect
) {
    assert(font); assert(ustr);
    if (!font || !ustr) return;

    const bool was_drawing_held = al_is_bitmap_drawing_held();
    al_hold_bitmap_drawing(true);

    float height = get_text_height(bounds.width, spacing, font, ustr);
    float x = bounds.x
        + float(align_x == AlignX::RIGHT) * bounds.width
        + float(align_x == AlignX::CENTER) * bounds.width / 2.f;
    float y = bounds.y
        + float(align_y == AlignY::BOTTOM) * (bounds.height - height)
        + float(align_y == AlignY::CENTER) * (bounds.height - height) / 2.f;

    auto data = MultilineData{x, y, bounds.height, font, color, align_x, spacing, effect};
    al_do_multiline_ustr(font, bounds.width, ustr, draw_fancy_text_helper, &data);

    al_hold_bitmap_drawing(was_drawing_held);
}

float get_text_height(float width, float spacing, const ALLEGRO_FONT *font, const char *cstr) {
    return get_text_line_count(width, font, cstr) * (al_get_font_line_height(font) + spacing) - spacing;
}

float get_text_height(float width, float spacing, const ALLEGRO_FONT *font, const ALLEGRO_USTR *ustr) {
    return get_text_line_count(width, font, ustr) * (al_get_font_line_height(font) + spacing) - spacing;
}

int get_text_line_count(float width, const ALLEGRO_FONT *font, const char *cstr) {
    int count = 0;
    al_do_multiline_text(font, width, cstr, get_cstr_line_count_helper, &count);
    return count;
}

int get_text_line_count(float width, const ALLEGRO_FONT *font, const ALLEGRO_USTR *ustr) {
    int count = 0;
    al_do_multiline_ustr(font, width, ustr, get_ustr_line_count_helper, &count);
    return count;
}

///////////////////////////////////////////////////////////////////////////////
/// TEXT EFFECTS

RainbowTextEffect::RainbowTextEffect(float shift) : shift(shift) {}

void RainbowTextEffect::operator()(int &ch, float &x, float &y, ALLEGRO_COLOR &color) {
    color = al_color_hsv(fmod(360.0f * sinf(x) + shift, 360.0f), 0.8f, 0.8f);
}

WavyTextEffect::WavyTextEffect(float length, float height, float shift)
    : wave_length(length), wave_height(height), wave_shift(shift)
{ }

void WavyTextEffect::operator()(int &ch, float &x, float &y, ALLEGRO_COLOR &color) {
    y += sinf(2.f * PI / wave_length * (x + wave_shift)) * wave_height / 2.f;
}

