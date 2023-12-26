#pragma once

#include <allegro5/allegro5.h>

#include "inputlog.hh"

class Mouse {
public:

    void handle_event(const ALLEGRO_EVENT &event) {
        switch (event.type) {
        case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
            log.press(event.mouse.button);
            break;
        case ALLEGRO_EVENT_MOUSE_BUTTON_UP:
            log.release(event.mouse.button);
            break;
        }
    }

    auto poll() {
        return log.poll();
    }

private:

    InputLog<2> log;

};