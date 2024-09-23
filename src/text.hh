#pragma once
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <functional>
#include "color.hh"
#include "font.hh"
#include "layout.hh"
#include "math.hh"
#include "string.hh"
#include "widget.hh"

using TextEffect = std::function<void(int, float &, float &, ALLEGRO_COLOR &)>;

// float find_text_height(ALLEGRO_FONT *font, const ALLEGRO_USTR *&string);
// float find_text_width(ALLEGRO_FONT *font, const ALLEGRO_USTR *&string);

void progress_animations();

void rainbow_text_effect(int line_num, float &x, float &y, ALLEGRO_COLOR color);

void wavy_text_effect(int line_num, float &x, float &y, ALLEGRO_COLOR color);

void draw_text(float x, float y,
    const ALLEGRO_FONT *font, ALLEGRO_COLOR color, const ALLEGRO_USTR *str,
    int align = ALIGN_LEFT, TextEffect per_char_cb = nullptr, int line_num = 0);

void draw_multiline_text(
    float x, float y, float max_width, float max_height,
    const ALLEGRO_FONT *font, ALLEGRO_COLOR color, const ALLEGRO_USTR *str,
    int align = ALIGN_LEFT, float spacing = 1.0f,
    TextEffect per_line_cb = nullptr, TextEffect per_char_cb = nullptr);

struct Text : Widget {

    Text(Rectangle area, Font font, Color color, String str, int align=ALIGN_LEFT, float spacing=1.0f);
    void render() const override;

    void resize_to_fit();
    int get_line_count() const;

    Font font;
    Color color;
    String str;
    float align;
    float spacing;

    TextEffect per_line_cb, per_char_cb;

};

