#include <cassert>
#include <iostream>
#include "text.hh"

///////////////////////////////////////////////////////////////////////////////
/// LOCAL API

namespace {

// problem: newline character doesn't result in hard break
// problem: characters caused by hard or soft break aren't passed to call
// problem: cursor at begining of line

// wanna do: advance index next on all characters in string

struct MultilineData {
    const float x, y, height, spacing;
    const Align::Horizontal align;
    const ALLEGRO_FONT *const font;
    std::function<void(int index, int ln, int ch, float x, float y)> call;
    float advance = 0.f; // vertical advance
    int index = 0;
};

bool do_textbox_per_char_helper(int ln, const ALLEGRO_USTR *ustr, void *extra) {
    MultilineData &data = *(MultilineData *)extra;
    if (!data.call) return false;

    float y = data.y + data.advance;
    if (y > data.y + data.height) return false;

    const float h = al_get_font_line_height(data.font);
    data.advance += h + data.spacing;
    if (y < data.y - h) {
        data.index += al_ustr_length(ustr)+1;
        return true;
    }

    const float w = al_get_ustr_width(data.font, ustr);
    float x = data.x
        - float(data.align == Align::RIGHT) * w
        - float(data.align == Align::CENTER_X) * w / 2.f;

    const auto draw_char = 
        [&](int ch, int advance) {
            x += advance;
            data.call(data.index++, ln, ch, x, y);
            return true;
        };
    
    ALLEGRO_USTR_INFO info;
    do_text_per_char(data.font, al_ref_ustr(&info, ustr, 0, al_ustr_size(ustr)), draw_char);
    
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

void do_text_per_char(
    const ALLEGRO_FONT *font, const char *cstr,
    std::function<bool(int ch, int advance)> call
) {
    assert(font); assert(cstr); assert(call);
    ALLEGRO_USTR_INFO info;
    do_text_per_char(font, al_ref_cstr(&info, cstr), call);
}

void do_text_per_char(
    const ALLEGRO_FONT *font, const ALLEGRO_USTR *ustr,
    std::function<bool(int ch, int advance)> call
) {
    assert(font); assert(ustr); assert(call);
    if (!font || !ustr || !call) return;
    int pos = al_ustr_offset(ustr, 0),
        prev_ch = ALLEGRO_NO_KERNING,
        ch = al_ustr_get_next(ustr, &pos);
    bool exit = false;
    while (!exit && ch != -1) {
        exit = !call(ch, al_get_glyph_advance(font, prev_ch, ch));
        prev_ch = ch;
        ch = al_ustr_get_next(ustr, &pos);
    }
    if (!exit) call(-1, al_get_glyph_advance(font, prev_ch, ALLEGRO_NO_KERNING));
}

void do_textbox_per_char(
    Rectangle textbox, Align align, float spacing, float scroll,
    const ALLEGRO_FONT *font, const char *cstr,
    std::function<void(int index, int ln, int ch, float x, float y)> call
) {
    assert(font); assert(cstr); assert(call);
    ALLEGRO_USTR_INFO info;
    do_textbox_per_char(textbox, align, spacing, scroll, font,
        al_ref_cstr(&info, cstr), call);
}

void do_textbox_per_char(
    Rectangle textbox, Align align, float spacing, float scroll,
    const ALLEGRO_FONT *font, const ALLEGRO_USTR *ustr,
    std::function<void(int index, int ln, int ch, float x, float y)> call
) {
    assert(font); assert(ustr); assert(call);

    // align the text horizontally within the textbox:
    float x = textbox.x
        + float(align.x == Align::RIGHT) * textbox.width
        + float(align.x == Align::CENTER_X) * textbox.width / 2.f;
    // align the text vertically within the textbox:
    float h = get_text_height(textbox.width, spacing, font, ustr);
    float y = textbox.y
        + float(align.y == Align::BOTTOM) * (textbox.height - h)
        + float(align.y == Align::CENTER_Y) * (textbox.height - h) / 2.f;
    
    auto data = MultilineData{x, y, textbox.height, spacing, align.x, font, call, scroll};
    al_do_multiline_ustr(font, textbox.width, ustr, do_textbox_per_char_helper, &data);
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

