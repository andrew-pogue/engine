#include <allegro5/allegro_color.h>
#include <cmath>
#include "math.hh"
#include "text-effect.hh"

RainbowTextEffect::RainbowTextEffect(float shift) : shift(shift) {}

void RainbowTextEffect::operator()(int index, int ln, int &ch, float &x, float &y, ALLEGRO_COLOR &color) {
    color = al_color_hsv(fmod(360.0f * sinf(x) + shift, 360.0f), 0.8f, 0.8f);
}

WavyTextEffect::WavyTextEffect(float length, float height, float shift)
    : wave_length(length), wave_height(height), wave_shift(shift)
{ }

void WavyTextEffect::operator()(int index, int ln, int &ch, float &x, float &y, ALLEGRO_COLOR &color) {
    y += sinf(2.f * PI / wave_length * (x + wave_shift)) * wave_height / 2.f;
}

