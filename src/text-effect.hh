#pragma once
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <concepts>
#include <functional>
#include <initializer_list>
#include "layout.hh"
#include "rectangle.hh"
#include "text.hh"

// multiple text effects can stack
// effects are applied in the order that they are declared
// effects mutate the x, y, ch, color before drawing

using TextEffectFunction =
    std::function<void(int index, int ln, int &ch, float &x, float &y, ALLEGRO_COLOR &color)>;

void draw_textbox_with_effects(
    Rectangle bounds, Align align, float spacing, float scroll,
    const ALLEGRO_FONT *font, ALLEGRO_COLOR color, const ALLEGRO_USTR *ustr,
    std::convertible_to<TextEffectFunction> auto... effects
) {
    assert(font); assert(ustr);
    const auto list = { (TextEffectFunction)effects... };
    const auto draw_char =
        [&](int index, int ln, int ch, float x, float y){
            for (TextEffectFunction effect : list)
                effect(index, ln, ch, x, y, color);
            if (ch > 0) al_draw_glyph(font, color, x, y, ch);
        };
    do_textbox_per_char(bounds, align, spacing, scroll, font, ustr, draw_char);
}

void draw_textbox_with_effects(
    Rectangle bounds, Align align, float spacing, float scroll,
    const ALLEGRO_FONT *font, ALLEGRO_COLOR color, const char *cstr,
    std::convertible_to<TextEffectFunction> auto... effects
) {
    assert(font); assert(cstr);
    ALLEGRO_USTR_INFO info;
    draw_textbox_with_effects(bounds, align, spacing, scroll, font, color,
        al_ref_cstr(&info, cstr), effects...);
}

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

