#pragma once
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <functional>
#include "layout.hh"
#include "rectangle.hh"

void draw_text_with_effect(float x, float y,
    const ALLEGRO_FONT *font, ALLEGRO_COLOR color, const char *cstr,
    std::function<void(int &ch, float &x, float &y, ALLEGRO_COLOR &color)> effect,
    int align = ALIGN_LEFT);

void draw_text_with_effect(float x, float y,
    const ALLEGRO_FONT *font, ALLEGRO_COLOR color, const ALLEGRO_USTR *ustr,
    std::function<void(int &ch, float &x, float &y, ALLEGRO_COLOR &color)> effect,
    int align = ALIGN_LEFT);

void draw_multiline_text_with_effect(Rectangle bounds,
    const ALLEGRO_FONT *font, ALLEGRO_COLOR color, const char *cstr,
    std::function<void(int ln, int &ch, float &x, float &y, ALLEGRO_COLOR &color)> effect,
    int align = ALIGN_LEFT, float spacing = 1.0f);

void draw_multiline_text_with_effect(Rectangle bounds,
    const ALLEGRO_FONT *font, ALLEGRO_COLOR color, const ALLEGRO_USTR *ustr,
    std::function<void(int ln, int &ch, float &x, float &y, ALLEGRO_COLOR &color)> effect,
    int align = ALIGN_LEFT, float spacing = 1.0f);

int get_text_line_count(float width, const ALLEGRO_FONT *font, const char *cstr);
int get_text_line_count(float width, const ALLEGRO_FONT *font, const ALLEGRO_USTR *ustr);

/*
 * make no room for type erasure or generics
 *
 * effect can be applied and removed from text widget
 * effects can be customized thus need data thus object
 * effects can be combined
 * */
