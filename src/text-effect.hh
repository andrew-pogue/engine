#pragma once
#include <allegro5/allegro.h>
#include <allegro5/allegro_color.h>
#include <cmath>
#include "math.hh"

struct TextEffect {

    virtual void operator()(int ln, int &ch, float &x, float &y, ALLEGRO_COLOR &color) = 0;

};

struct RainbowTextEffect : TextEffect {

    float shift;

    RainbowTextEffect(float progress) : shift(progress) {}

    void operator()(int ln, int &ch, float &x, float &y, ALLEGRO_COLOR &color) override {
        color = al_color_hsv(fmod(360.0f * sinf(x) + shift, 360.0f), 0.8f, 0.8f);
    }

};

struct WavyTextEffect : TextEffect {

    float wave_length; // pixels
    float wave_height; // pixels
    float wave_shift; // pixels

    WavyTextEffect(float length, float height, float shift=0.f)
        : wave_length(length), wave_height(height), wave_shift(shift)
    { }

    void operator()(int ln, int &ch, float &x, float &y, ALLEGRO_COLOR &color) override {
        y += sinf(2.f * PI / wave_length * (x + wave_shift) + (PI / 4.f * ln)) * wave_height / 2.f;
        // y += sinf((PI / 4.f * ln) + (2.f * PI * x / wave_length) + wave_shift) * wave_height / 2.f;
    }

};

