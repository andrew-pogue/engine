#pragma once
#include <allegro5/allegro.h>
#include "rectangle.hh"
#include "theme.hh"

// a graphical user-interface element
struct Widget {

    float x, y, width, height;

    Widget(Rectangle area) : x{area.x}, y{area.y}, width{area.width}, height{area.height} { }
    Widget(float x, float y, float w, float h) : x{x}, y{y}, width{w}, height{h} { }

    virtual bool handle_event(const ALLEGRO_EVENT &event) = 0;
    // returns false if exit condition was met
    virtual bool update(double dt) { return true; }
    virtual void render(Theme theme) const = 0;

    Rectangle area() const { return { x, y, width, height }; }

};

