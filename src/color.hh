#pragma once
#include <allegro5/allegro.h>
#include <allegro5/allegro_color.h>

// wrapper class for ALLEGRO_COLOR
// mostly so that (const char *) can implicitly convert to ALLEGRO_COLOR, so syntactic sugar
struct Color {

    Color(ALLEGRO_COLOR color) : color_(color) {}
    Color(const char *name) : color_(al_color_name(name)) {}
    Color(int r, int g, int b) : color_(al_map_rgb(r,g,b)) {}
    Color(int r, int g, int b, int a) : color_(al_map_rgba(r,g,b,a)) {}
    Color(float r, float g, float b) : color_(al_map_rgb_f(r,g,b)) {}
    Color(float r, float g, float b, float a) : color_(al_map_rgba_f(r,g,b,a)) {}

    operator ALLEGRO_COLOR &() { return color_; }
    operator ALLEGRO_COLOR() const { return color_; }

private:

    ALLEGRO_COLOR color_;

};

