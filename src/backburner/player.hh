#pragma once

#include <allegro5/allegro5.h>

#include "inputlog.hh"

class Player {
public:

    void handle_event(const ALLEGRO_EVENT &event) {
        switch (event.type) {
        case ALLEGRO_EVENT_KEY_DOWN:
            keyboard.press(event.keyboard.keycode);
            break;
        case ALLEGRO_EVENT_KEY_UP:
            keyboard.release(event.keyboard.keycode);
            break;
        }
    }

    void update() {
        auto input = keyboard.poll();
        if (input[controls.move_left])
            position.x -= 1;
        if (input[controls.move_right])
            position.x += 1;
        if (input[controls.move_up])
            position.y -= 1;
        if (input[controls.move_down])
            position.y += 1;
    }

    struct {
        int move_left = ALLEGRO_KEY_A,
            move_right = ALLEGRO_KEY_D,
            move_up = ALLEGRO_KEY_W,
            move_down = ALLEGRO_KEY_S;
    } controls;

    struct {
        int x, y;
    } position;

private:

    InputLog<ALLEGRO_KEY_MAX> keyboard;

};