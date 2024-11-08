#pragma once
#include <allegro5/allegro.h>
#include <bitset>
#include <cassert>

struct KeyboardLog {

    static const char *key_name(int key) {
        assert(key > 0 && key < ALLEGRO_KEY_MAX && "[Keyboard] invalid key passed");
        return al_keycode_to_name(key);
    }

    KeyboardLog(ALLEGRO_DISPLAY *display);
    
    bool handle_event(const ALLEGRO_EVENT &event);

    // marks the status of all keys as old (has not changed)
    // and returns a bitset of was-pressed values for every key
    std::bitset<ALLEGRO_KEY_MAX> poll();

    // marks the status of all keys as old (has not changed)
    void clear_changes() { has_changed_.reset(); }

    void clear() { is_pressed_.reset(); has_changed_.reset(); }

    // returns true if the key was pressed since the last poll
    bool was_pressed(int key) const {
        assert(key > 0 && key < ALLEGRO_KEY_MAX && "[Keyboard] invalid key passed");
        return is_pressed_.test(key) || has_changed_.test(key);
    }

    // returns true if the key remained pressed since the last poll
    bool was_held(int key) const {
        assert(key > 0 && key < ALLEGRO_KEY_MAX && "[Keyboard] invalid key passed");
        return is_pressed_.test(key) && !has_changed_.test(key);
    }

    // returns true if the key is currently pressed
    bool is_pressed(int key) const {
        assert(key > 0 && key < ALLEGRO_KEY_MAX && "[Keyboard] invalid key passed");
        is_pressed_.test(key);
    }

    // returns true if the key was pressed or released since the last poll
    // can be used to determine if a keys status is new or old
    bool has_changed(int key) const {
        assert(key > 0 && key < ALLEGRO_KEY_MAX && "[Keyboard] invalid key passed");
        return has_changed_.test(key);
    }

    // returns true if the key was pressed since the last poll
    bool operator[](int key) const { return was_pressed(key); }

    const ALLEGRO_DISPLAY *const display; // non-owning

private:

    std::bitset<ALLEGRO_KEY_MAX> is_pressed_, has_changed_;

};

