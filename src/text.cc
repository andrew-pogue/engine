#include <cassert>
#include "text.hh"

///////////////////////////////////////////////////////////////////////////////
/// LOCAL API

namespace {

struct MultilineData {
    const Rectangle bounds;
    const ALLEGRO_FONT *font;
    const ALLEGRO_COLOR color;
    const int align;
    const float spacing;
    const std::function<void(int ln, int &ch, float &x, float &y, ALLEGRO_COLOR &color)> effect;
    float advance = 0.f;
};

bool draw_multiline_text_helper(int ln, const ALLEGRO_USTR *str, void *extra) {
    MultilineData &data = *(MultilineData *)extra;

    auto effect = std::bind_front(data.effect, ln);

    float y = data.bounds.y + data.advance;
    if (y < data.bounds.y) return true;
    if (y > data.bounds.y + data.bounds.height) return false;

    float x = data.bounds.x;
    if (data.align == ALIGN_RIGHT) x += data.bounds.width;
    else if (data.align == ALIGN_CENTER) x += data.bounds.width / 2.f;
    
    ALLEGRO_USTR_INFO info;
    draw_text_with_effect(x, y,
        data.font, data.color,
        al_ref_ustr(&info, str, 0, al_ustr_size(str)),
        effect, data.align);
    
    data.advance += al_get_font_line_height(data.font) + data.spacing;
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

void draw_text_with_effect(
    float x, float y,
    const ALLEGRO_FONT *font, ALLEGRO_COLOR color, const char *cstr,
    std::function<void(int &ch, float &x, float &y, ALLEGRO_COLOR &color)> effect,
    int align
) {
    static auto draw_char =
        [&effect, font](int ch, float x, float y, ALLEGRO_COLOR color) {
            // changes made by effect are scoped to this lambda
            if (effect) effect(ch, x, y, color);
            al_draw_glyph(font, color, x, y, ch);
        };

    assert(font); assert(cstr);
    if (!font || !cstr) return;

    const bool was_drawing_held = al_is_bitmap_drawing_held();
    al_hold_bitmap_drawing(true);

    if (align == ALIGN_CENTER) x -= al_get_text_width(font, cstr) / 2.0f;
    else if (align == ALIGN_RIGHT) x -= al_get_text_width(font, cstr);

    char prev_ch = cstr[0];
    draw_char(prev_ch, x, y, color);

    for (size_t pos=1u; cstr[pos] != '\0'; pos++) {
        x += al_get_glyph_advance(font, prev_ch, cstr[pos]);
        draw_char(cstr[pos], x, y, color);
        prev_ch = cstr[pos];
    }

    al_hold_bitmap_drawing(was_drawing_held);
}

void draw_text_with_effect(
    float x, float y,
    const ALLEGRO_FONT *font, ALLEGRO_COLOR color, const ALLEGRO_USTR *ustr,
    std::function<void(int &ch, float &x, float &y, ALLEGRO_COLOR &color)> effect,
    int align
) {
    static auto draw_char =
        [&effect, font](int ch, float x, float y, ALLEGRO_COLOR color) {
            // changes made by effect are scoped to this lambda
            if (effect) effect(ch, x, y, color);
            al_draw_glyph(font, color, x, y, ch);
        };

    assert(font); assert(ustr);
    if (!font || !ustr) return;

    const bool was_drawing_held = al_is_bitmap_drawing_held();
    al_hold_bitmap_drawing(true);

    if (align == ALIGN_CENTER) x -= al_get_ustr_width(font, ustr) / 2.0f;
    else if (align == ALIGN_RIGHT) x -= al_get_ustr_width(font, ustr);

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

    al_hold_bitmap_drawing(was_drawing_held);
}

void draw_multiline_text_with_effect(
    Rectangle bounds,
    const ALLEGRO_FONT *font, ALLEGRO_COLOR color, const char *cstr,
    std::function<void(int ln, int &ch, float &x, float &y, ALLEGRO_COLOR &color)> effect,
    int align, float spacing
) {
    assert(font); assert(cstr);
    if (!font || !cstr) return;
    auto ustr = al_ustr_new(cstr);
    draw_multiline_text_with_effect(bounds, font, color, ustr, effect, align, spacing);
    al_ustr_free(ustr);
}

void draw_multiline_text_with_effect(
    Rectangle bounds,
    const ALLEGRO_FONT *font, ALLEGRO_COLOR color, const ALLEGRO_USTR *ustr,
    std::function<void(int ln, int &ch, float &x, float &y, ALLEGRO_COLOR &color)> effect,
    int align, float spacing
) {
    assert(font); assert(ustr);
    if (!font || !ustr) return;

    int px, py, pw, ph;
    al_get_clipping_rectangle(&px, &py, &pw, &ph);
    al_set_clipping_rectangle(bounds.x, bounds.y, bounds.width, bounds.height);
    const bool was_drawing_held = al_is_bitmap_drawing_held();
    al_hold_bitmap_drawing(true);

    auto data = MultilineData{bounds, font, color, align, spacing, effect};
    al_do_multiline_ustr(font, bounds.width, ustr, draw_multiline_text_helper, &data);

    al_set_clipping_rectangle(px, py, pw, ph);
    al_hold_bitmap_drawing(was_drawing_held);
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

