#pragma once
#include <cassert>
#include "actor.hh"
#include "keyboard.hh"

struct Context {
    
    virtual void handle_event(const ALLEGRO_EVENT &event) = 0;
    virtual bool update(double dt) = 0;

};

struct PlayerContext : Context {

    PlayerContext(ALLEGRO_DISPLAY *display, Actor *player);

    void handle_event(const ALLEGRO_EVENT &event) override { keyboard_.handle_event(event); }

    bool update(double dt) override;

    Actor *player;

    struct {
        int move_up = ALLEGRO_KEY_UP,
            move_down = ALLEGRO_KEY_DOWN,
            move_right = ALLEGRO_KEY_RIGHT,
            move_left = ALLEGRO_KEY_LEFT;
    } keycommands;

private:

    KeyboardLog keyboard_;
    double repeat_delay_;

};

