#pragma once
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <functional>
#include "layout.hh"
#include "rectangle.hh"

using TextEffect = std::function<void(int &ch, float &x, float &y, ALLEGRO_COLOR &color)>;

void draw_text(Rectangle bounds, AlignX align_x, AlignY align_y,
    const ALLEGRO_FONT *font, ALLEGRO_COLOR color, const char *cstr,
    float spacing, TextEffect effect=nullptr);

void draw_text(Rectangle bounds, AlignX align_x, AlignY align_y,
    const ALLEGRO_FONT *font, ALLEGRO_COLOR color, const ALLEGRO_USTR *ustr,
    float spacing, TextEffect effect=nullptr);

float get_text_height(float width, float spacing, const ALLEGRO_FONT *font, const char *cstr);
float get_text_height(float width, float spacing, const ALLEGRO_FONT *font, const ALLEGRO_USTR *ustr);

int get_text_line_count(float width, const ALLEGRO_FONT *font, const char *cstr);
int get_text_line_count(float width, const ALLEGRO_FONT *font, const ALLEGRO_USTR *ustr);

struct RainbowTextEffect {

    float shift;

    RainbowTextEffect(float shift=0.f);
    void operator()(int &ch, float &x, float &y, ALLEGRO_COLOR &color);

};

struct WavyTextEffect {

    float wave_length, wave_height, wave_shift;

    WavyTextEffect(float length, float height, float shift=0.f);
    void operator()(int &ch, float &x, float &y, ALLEGRO_COLOR &color);

};

