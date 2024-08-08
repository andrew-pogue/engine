#pragma once
#include <allegro5/allegro.h>
#include <cassert>

enum {
    LEFT_MOUSE_BUTTON = 1,
    RIGHT_MOUSE_BUTTON = 2,
    MIDDLE_MOUSE_BUTTON = 4
};

class Mouse {
private:

    int x_, y_, dz_, dw_;
    unsigned buttons_;
    bool ignore_; 

public:
    
    struct Poll { int x, y, dz, dw; };

    const ALLEGRO_DISPLAY *const display;
    const int &x, &y;

    Mouse(ALLEGRO_DISPLAY *display)
        : x_(0), y_(0), dz_(0), dw_(0)
        , buttons_(0u)
        , display(display), enable(display)
        , x(x_), y(y_)
    { 
        assert(al_is_mouse_installed() && "[Mouse] mouse driver is not installed");
        assert(display != nullptr && "[Mouse] display cannot be null");
    }

    void handle_event(const ALLEGRO_EVENT &event) {
        switch (event.type) {
        case ALLEGRO_EVENT_MOUSE_AXES:
            if (!ignore_ && event.mouse.display == display) {
                x_ = event.mouse.x;
                y_ = event.mouse.y;
                z_ += event.mouse.dz;
                w_ += event.mouse.dw;
            } break;
        case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
            if (!ignore_ && event.mouse.display == display) {
                buttons_ |= event.mouse.button;
            } break;
        case ALLEGRO_EVENT_MOUSE_BUTTON_UP:
            if (!ignore_ && event.mouse.display == display) {
                buttons_ &= !event.mouse.button;
            } break;
        case ALLEGRO_EVENT_MOUSE_ENTER_DISPLAY:
            if (event.mouse.display == display) {
                ignore_ = true;
                x_ = event.mouse.x;
                y_ = event.mouse.y;
            } break;
        case ALLEGRO_EVENT_MOUSE_LEAVE_DISPLAY:
            if (event.mouse.display == display) {
                ignore_ = false;
            } break;
        }
    }
    
    Poll poll() {
        Poll ret{ x_, y_, dz_, dw_; };
        dz_ = 0; dw_ = 0;
        return ret; 
    }

    bool is_button_pressed(int button) const {
        return buttons_ & button;
    }

};

