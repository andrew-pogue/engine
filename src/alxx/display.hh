#pragma once
#include <allegro5/allegro5.h>

namespace alxx {

class Display {
public:

    Display(int width, int height) 
        : display(al_create_display(width,height))
    {
        if (!display) throw "Failed to create display.";
    }

    ~Display() {
        if (display) al_destroy_display(display);
    }

    auto get_event_source() {
        return al_get_display_event_source(display);
    }

    bool acknowledge_resize() {
        return al_acknowledge_resize(display);
    }

    auto get_backbuffer() {
        return al_get_backbuffer(display);
    }

private:

    ALLEGRO_DISPLAY *display;

};

} // namespace alxx