#pragma once
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <functional>
#include "layout.hh"
#include "rectangle.hh"

void do_text_per_char(
    const ALLEGRO_FONT *font, const char *cstr,
    std::function<bool(int ch, int advance)> call);

void do_text_per_char(
    const ALLEGRO_FONT *font, const ALLEGRO_USTR *ustr,
    std::function<bool(int ch, int advance)> call);

void do_textbox_per_char(
    Rectangle bounds, Align align, float spacing, float scroll,
    const ALLEGRO_FONT *font, const char *cstr,
    std::function<void(int index, int ln, int ch, float x, float y)> call);

void do_textbox_per_char(
    Rectangle bounds, Align align, float spacing, float scroll,
    const ALLEGRO_FONT *font, const ALLEGRO_USTR *ustr,
    std::function<void(int index, int ln, int ch, float x, float y)> call);

float get_text_height(float width, float spacing, const ALLEGRO_FONT *font, const char *cstr);
float get_text_height(float width, float spacing, const ALLEGRO_FONT *font, const ALLEGRO_USTR *ustr);

int get_text_line_count(float width, const ALLEGRO_FONT *font, const char *cstr);
int get_text_line_count(float width, const ALLEGRO_FONT *font, const ALLEGRO_USTR *ustr);


