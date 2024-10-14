#pragma once
#include <allegro5/allegro.h>
#include "rectangle.hh"

// a graphical user-interface element
struct Widget {

    Rectangle bounds;

    Widget(Rectangle area) : bounds(area) { }
    Widget(float x, float y, float w, float h) : bounds(x, y, w, h) { }

    virtual bool handle_event(const ALLEGRO_EVENT &event) = 0;
    // returns false if exit condition was met
    virtual bool update(double dt) { return true; }
    virtual void render() const = 0;

};

