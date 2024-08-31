#pragma once
#include <allegro5/allegro.h>
#include <cassert>
#include <functional>

// wrapper class for ALLEGRO_DISPLAY
class Display {
private:

    ALLEGRO_DISPLAY *const display_;

    // creates a display with the given values without replacing the global new-display values 
    ALLEGRO_DISPLAY *create_display_with(int width, int height, int flags, const char *title) {
        int default_flags = al_get_new_display_flags();
        const char *default_title = al_get_new_window_title();
        al_set_new_display_flags(flags);
        al_set_new_window_title(title);
        ALLEGRO_DISPLAY *display = al_create_display(width, height);
        al_set_new_display_flags(default_flags);
        al_set_new_window_title(default_title);
        return display;
    }

public:

    Display(int width, int height, int flags, const char *title=nullptr)
        : display_(create_display_with(width, height, flags, title))
    { assert(display_ && "failed to create display"); }

    Display(int width, int height, const char *title=nullptr)
        : display_(al_create_display(width, height))
    {
        assert(display_ && "failed to create display");
        if (title) set_title(title);
    }

    // "You should make sure no threads are currently targeting a
    // bitmap which is tied to this display before you destroy it."
    ~Display() { al_destroy_display(display_); }
        
    bool handle_event(const ALLEGRO_EVENT &event) {
        switch (event.type) {
        case ALLEGRO_EVENT_DISPLAY_RESIZE:
            if (is_handled = event.display.source == display_) {
                al_acknowledge_resize(display_);
            } break;
        case ALLEGRO_EVENT_DISPLAY_SWITCH_OUT:
            if (is_handled = event.display.source == display_) {
                if (al_is_keyboard_installed()) al_clear_keyboard_state(display_);
            } break;
        }
        return false;
    }

    int get_height() const { return al_get_display_height(display_); }
    int get_width() const { return al_get_display_width(display_); }
    auto get_backbuffer() { return al_get_backbuffer(display_); }
    void set_title(const char *title) { al_set_window_title(display_, title); }

    void resize(int width, int height) { al_resize_display(display_, width, height); }
    
    bool operator==(const ALLEGRO_DISPLAY *other) const { return other == display_; }
    bool operator==(const Display &other) const { return other.display_ == display_; }

    ALLEGRO_DISPLAY *get() { return display_; }
    ALLEGRO_EVENT_SOURCE *get_event_source() const { return al_get_display_event_source(display_); }

};

