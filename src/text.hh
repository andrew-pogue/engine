#pragma once
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <concepts>
#include <functional>
#include "layout.hh"
#include "rectangle.hh"

using TextEffectCall =
    std::function<void(int index, int ln, int &ch, float &x, float &y, ALLEGRO_COLOR &color)>;

void do_per_char(
    const ALLEGRO_FONT *font, const char *cstr,
    std::function<bool(int ch, int advance)> call);

void do_per_char(
    const ALLEGRO_FONT *font, const ALLEGRO_USTR *ustr,
    std::function<bool(int ch, int advance)> call);

void draw_text_with_effects(
    Rectangle bounds, Align align, float spacing, float scroll,
    const ALLEGRO_FONT *font, ALLEGRO_COLOR color, const char *cstr,
    std::convertible_to<TextEffectCall> auto... effects);

void draw_text_with_effects(
    Rectangle bounds, Align align, float spacing, float scroll,
    const ALLEGRO_FONT *font, ALLEGRO_COLOR color, const ALLEGRO_USTR *ustr,
    std::convertible_to<TextEffectCall> auto... effects);

// std::function<void(int ln, float x, float y)> per_line_call,
// std::function<void(int ch, float x, float y)> per_char_call
// void *extra

float get_text_height(float width, float spacing, const ALLEGRO_FONT *font, const char *cstr);
float get_text_height(float width, float spacing, const ALLEGRO_FONT *font, const ALLEGRO_USTR *ustr);

int get_text_line_count(float width, const ALLEGRO_FONT *font, const char *cstr);
int get_text_line_count(float width, const ALLEGRO_FONT *font, const ALLEGRO_USTR *ustr);

struct TextEffect {

    virtual void operator()(int index, int ln, int &ch, float &x, float &y, ALLEGRO_COLOR &color) = 0;

};

struct RainbowTextEffect : TextEffect {

    float shift;

    RainbowTextEffect(float shift=0.f);
    void operator()(int index, int ln, int &ch, float &x, float &y, ALLEGRO_COLOR &color) override;

};

struct WavyTextEffect : TextEffect {

    float wave_length, wave_height, wave_shift;

    WavyTextEffect(float length, float height, float shift=0.f);
    void operator()(int index, int ln, int &ch, float &x, float &y, ALLEGRO_COLOR &color) override;

};

