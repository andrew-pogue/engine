#pragma once

// a graphical user-interface element
struct Widget {

    float preferred_width, preferred_height;

    Widget(float width, float height)
        : preferred_width(width), preferred_height(height)
    {}

    virtual ~Widget() {}

    // returns true if event was handled
    virtual bool handle_event(const ALLEGRO_EVENT &event) { return false };
    // returns false if exit condition was met
    virtual bool update(double dt) { return true; };
    virtual void render(float x, float y) const = 0;

};

