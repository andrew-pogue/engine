#include <allegro5/allegro_color.h>
#include <cassert>
#include <cmath>
#include <initializer_list>
#include "math.hh"
#include "text.hh"

///////////////////////////////////////////////////////////////////////////////
/// LOCAL API

namespace {

// problem: newline character doesn't result in hard break
// problem: characters caused by hard or soft break aren't passed to call
// problem: cursor at begining of line

// wanna do: advance index next on all characters in string

struct DrawGlyphInfo {
    float x, y;
};

struct MultilineData {
    const Rectangle textbox;
    const AlignX align;
    const float spacing;
    const ALLEGRO_FONT *const font;
    ALLEGRO_COLOR color;
    std::initializer_list<TextEffectCall *> effects;
    float advance = 0.f; // vertical advance
    int index = 0;
};

bool draw_text_with_effects_helper(int ln, const ALLEGRO_USTR *ustr, void *extra) {
    MultilineData &data = *(MultilineData *)extra;
    data.index += int(ln != 0); // account for the deliminator character

    float y = data.textbox.y + data.advance;
    if (y > data.textbox.y + data.textbox.height) return false;

    const float h = al_get_font_line_height(data.font);
    data.advance += h + data.spacing;
    if (y < data.textbox.y - h) { data.index += al_ustr_length(ustr); return true; }

    const float w = al_get_ustr_width(data.font, ustr);
    float x = data.textbox.x
        - float(data.align == AlignX::RIGHT) * w
        - float(data.align == AlignX::CENTER) * w / 2.f;

    const auto draw_char = 
        [&](int ch, int advance) {
            x += advance;
            for (auto effect : data.effects) (*effect)(data.index, ln, ch, x, y, data.color);
            if (ch > 0) al_draw_glyph(data.font, data.color, x, y, ch);
            return true;
        };
    
    ALLEGRO_USTR_INFO info;
    do_per_char(data.font, al_ref_ustr(&info, ustr, 0, al_ustr_size(ustr)), draw_char);
    
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

// RULES:
// multiple text effects can stack
// effects are applied in the order that they are declared
// effects mutate the x, y, ch, color before drawing

void do_per_char(
    const ALLEGRO_FONT *font, const char *cstr,
    std::function<bool(int ch, int advance)> call
) {
    assert(font); assert(cstr); assert(call);
    ALLEGRO_USTR_INFO info;
    do_per_char(font, al_ref_cstr(&info, cstr), call);
}

void do_per_char(
    const ALLEGRO_FONT *font, const ALLEGRO_USTR *ustr,
    std::function<bool(int ch, int advance)> call
) {
    assert(font); assert(ustr); assert(call);

    int pos = al_ustr_offset(ustr, 0),
        prev_ch = ALLEGRO_NO_KERNING,
        ch = al_ustr_get(ustr, pos);
    if (ch == -1) return;
    while (
        call(ch, al_get_glyph_advance(font, prev_ch, ch))
        && al_ustr_next(ustr, &pos)
    ) {
        ch = al_ustr_get(ustr, pos);
        prev_ch = ch;
    } 
}

void draw_text_with_effects(
    Rectangle textbox, Align align, float spacing, float scroll,
    const ALLEGRO_FONT *font, ALLEGRO_COLOR color, const char *cstr,
    std::convertible_to<TextEffectCall> auto... effects
) {
    assert(font); assert(cstr);
    ALLEGRO_USTR_INFO info;
    draw_text_with_effects(textbox, align, spacing, scroll, font, color,
        al_ref_cstr(&info, cstr), effects...);
}

void draw_text_with_effects(
    Rectangle textbox, Align align, float spacing, float scroll,
    const ALLEGRO_FONT *font, ALLEGRO_COLOR color, const ALLEGRO_USTR *ustr,
    std::convertible_to<TextEffectCall> auto... effects
) {
    assert(font); assert(ustr);

    // align the text horizontally within the textbox:
    float x = textbox.x
        + float(align.x == AlignX::RIGHT) * textbox.width
        + float(align.x == AlignX::CENTER) * textbox.width / 2.f;
    // align the text vertically within the textbox:
    float h = get_text_height(textbox.width, spacing, font, ustr);
    float y = textbox.y
        + float(align.y == AlignY::BOTTOM) * (textbox.height - h)
        + float(align.y == AlignY::CENTER) * (textbox.height - h) / 2.f;
    
    auto data = MultilineData{textbox, align.x, spacing, font,
        { (TextEffectCall)effects... }, scroll};
    al_do_multiline_ustr(font, textbox.width, ustr, draw_text_with_effects_helper, &data);
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

void RainbowTextEffect::operator()(int index, int ln, int &ch, float &x, float &y, ALLEGRO_COLOR &color) {
    color = al_color_hsv(fmod(360.0f * sinf(x) + shift, 360.0f), 0.8f, 0.8f);
}

WavyTextEffect::WavyTextEffect(float length, float height, float shift)
    : wave_length(length), wave_height(height), wave_shift(shift)
{ }

void WavyTextEffect::operator()(int index, int ln, int &ch, float &x, float &y, ALLEGRO_COLOR &color) {
    y += sinf(2.f * PI / wave_length * (x + wave_shift)) * wave_height / 2.f;
}

