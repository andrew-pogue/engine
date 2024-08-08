#pragma once
#include <allegro5/allegro.h>
#include <cassert>

// wrapper class for ALLEGRO_DISPLAY
class Display {
private:

    ALLEGRO_DISPLAY *const display_;

public:

    Display(int width, int height, const char *title=nullptr)
        : display_(al_create_display(width, height))
    {
        assert(display_ && "failed to create display");
        set_title(title);
    }

    // "You should make sure no threads are currently targeting a
    // bitmap which is tied to this display before you destroy it."
    ~Display() { al_destroy_display(display_); }
        
    void handle_event(const ALLEGRO_EVENT &event) {
        switch (event.type) {
        case ALLEGRO_EVENT_DISPLAY_RESIZE:
            if (event.display.source == display) {
                al_acknowledge_resize(display_);
            } break;
        case ALLEGRO_EVENT_DISPLAY_LOST:
            if (event.display.source == display_) {
                std::cerr << "[Display::handle_event] The display was lost??\n";
            } break;
        case ALLEGRO_EVENT_DISPLAY_FOUND:
            if (event.display.source == display_) {
                std::cerr << "[Display::handle_event] The display was found.\n";
            } break;
        }
    }

    int get_height() const { return al_get_display_height(display_); }
    int get_width() const { return al_get_display_width(display_); }
    auto get_backbuffer() { return al_get_backbuffer(display); }
    void set_title(const char *title) { al_set_window_title(display_, title_); }

    void resize(int width, int height) { al_resize_display(display_, width, height); }
    
    bool operator==(const ALLEGRO_DISPLAY *other) const { return other == display_; }
    bool operator==(const Display &other) const { return other.display_ == display_; }

    ALLEGRO_DISPLAY *get() { return display_; }
    ALLEGRO_EVENT_SOURCE *get_event_source() const { return al_get_display_event_source(display_); }

};

