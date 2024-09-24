#include <allegro5/allegro_color.h>
#include <cassert>
#include <cmath>
#include "text.hh"

///////////////////////////////////////////////////////////////////////////////
/// LOCAL API

namespace {

    unsigned tick = 0u;

    struct MultilineData {
        const ALLEGRO_FONT *font;
        ALLEGRO_COLOR color;
        float x, y, w, h; // bouding box paramters
        int align;
        float spacing;
        TextEffect per_line_cb, per_char_cb;
    };

    bool draw_multiline_helper(int line_num, const ALLEGRO_USTR *str, void *line_help) {
        const MultilineData &data = *(MultilineData *)line_help;

        float y = data.y + line_num * (al_get_font_line_height(data.font) + data.spacing);
        if (y < data.y) return true;
        if (y > data.y + data.h) return false;

        float x = data.x;
        if (data.align == ALIGN_RIGHT)  x += data.w;
        if (data.align == ALIGN_CENTER) x += data.w / 2.0f;

        auto color = data.color;
        if (data.per_line_cb) data.per_line_cb(line_num, x, y, color);

        ALLEGRO_USTR_INFO info;
        draw_text(x, y, data.font, data.color, al_ref_ustr(&info, str, 0, al_ustr_size(str)), data.align, data.per_char_cb, line_num); 
        return true;
    }

    bool get_line_count_helper(int line_num, const ALLEGRO_USTR *str, void *count) {
        *(int *)count = line_num + 1;
        return true;
    }

} // namespace

///////////////////////////////////////////////////////////////////////////////
/// GLOBAL API

void progress_animations() { tick++; }
    
void rainbow_text_effect(int line_num, float &x, float &y, ALLEGRO_COLOR &color) {
    color = al_color_hsv(fmod(360.0f * sinf(x) + tick, 360.0f), 0.8f, 0.8f);
}

void wavy_text_effect(int line_num, float &x, float &y, ALLEGRO_COLOR &color) {
    const float amplitude=4.0f, speed=5.0f, frequency=5.0f;
    y += sinf(line_num * 0.75f + (x * frequency + tick * speed) * TO_RADIANS) * amplitude;
}

void draw_text(
    float x, float y, const ALLEGRO_FONT *font, ALLEGRO_COLOR color, const ALLEGRO_USTR *str,
    int align, TextEffect per_char_cb, int line_num
) {
    assert(font); assert(str);

    const bool was_drawing_held = al_is_bitmap_drawing_held();
    al_hold_bitmap_drawing(true);

    if (align == ALIGN_CENTER) x -= al_get_ustr_width(font, str) / 2.0f;
    if (align == ALIGN_RIGHT) x -= al_get_ustr_width(font, str);

    int pos = al_ustr_offset(str, 0),
        prev_ch = 0;
    bool first = true;

    do {
        int ch = al_ustr_get(str, pos);
        x += first ? 0.0f : al_get_glyph_advance(font, prev_ch, ch);
        if (ch >= 0) {
            float i = x, j = y; auto c = color;
            if (per_char_cb) per_char_cb(line_num, i, j, c);
            al_draw_glyph(font, c, i, j, ch);
        }
        prev_ch = ch; first = false;
    } while (al_ustr_next(str, &pos));

    al_hold_bitmap_drawing(was_drawing_held);
}

void draw_multiline_text(
    float x, float y, float w, float h,
    const ALLEGRO_FONT *font, ALLEGRO_COLOR color, const ALLEGRO_USTR *str,
    int align, float spacing, TextEffect per_line_cb, TextEffect per_char_cb
) {
    assert(font); assert(str);
    const bool was_drawing_held = al_is_bitmap_drawing_held();
    al_hold_bitmap_drawing(true);

    auto data = MultilineData{font, color, x, y, w, h, align, spacing, per_line_cb, per_char_cb};
    al_do_multiline_ustr(font, w, str, draw_multiline_helper, &data);

    al_hold_bitmap_drawing(was_drawing_held);
}

///////////////////////////////////////////////////////////////////////////////
/// PUBLIC API

Text::Text(Rectangle area, ALLEGRO_FONT *font, ALLEGRO_COLOR color, const char *str, int align, float spacing)
    : Widget(area), font(font), color(color), str(al_ustr_new(str)), align(align), spacing(spacing)
{ }

Text::~Text() { al_ustr_free(str); }

void Text::resize_to_fit() {
    height = get_line_count() * (al_get_font_line_height(font) + spacing) - spacing;
}

void Text::render() const {
    draw_multiline_text(x, y, width, height, font, color, str, align, spacing, per_line_cb, per_char_cb);
}

int Text::get_line_count() const {
    int count = 0;
    al_do_multiline_ustr(font, width, str, get_line_count_helper, &count);
    return count;
}

