#pragma once
#include <allegro5/allegro_primitives.h>

class Rectangle {
public:

    Rectangle(float width, float height, ALLEGRO_COLOR color,
        float thickness=0.0f, float rx=0.0f, float ry=0.0f)
        : width(width), height(height)
        , border{thickness>0.0f, color, thickness}
        , fill{thickness<=0.0f, color}
        , rounding{rx, ry}
    {
    }

    void render(float x, float y) const {
        if (fill.enabled)
            al_draw_filled_rounded_rectangle(
                x, y,
                x+width, y+height,
                rounding.x, rounding.y,
                fill.color
            );
        if (border.enabled)
            al_draw_rounded_rectangle(
                x, y,
                x+width, y+height,
                rounding.x, rounding.y,
                border.color,
                border.thickness
            );
    }

    float width, height;

    struct {
        bool enabled;
        ALLEGRO_COLOR color;
        float thickness;
    } border;

    struct {
        bool enabled;
        ALLEGRO_COLOR color;
    } fill;

    struct {
        float x, y;
    } rounding;

};