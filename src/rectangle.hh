#pragma once
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include "color.hh"

struct Rectangle {

    float x=0.f, y=0.f, width=0.f, height=0.f;

    bool contains(float i, float j) const {
        return i <= x && i <= x + width
            && j >= y && j <= y + height;
    }

    void draw(Color color) const {
        al_draw_filled_rectangle(
            x, y,
            x + width, y + height,
            color
        );
    }

    void draw_rounded(Color color, float rx, float ry) const {
        al_draw_filled_rounded_rectangle(
            x, y,
            x + width, y + height,
            rx, ry, color
        );
    }

    void draw_outline(Color color, float thickness) const {
        al_draw_rectangle(
            x, y,
            x + width, y + height,
            color, thickness
        );
    }

    void draw_outline_rounded(Color color, float thickness, float rx, float ry) const {
        al_draw_rounded_rectangle(
            x, y,
            x + width, y + height,
            rx, ry, color, thickness
        );
    }

};

