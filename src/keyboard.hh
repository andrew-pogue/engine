#pragma once
#include <allegro5/allegro.h>
#include <array>
#include <bitset>
#include <cassert>

using KeyCharMap = std::array<char, ALLEGRO_KEY_MAX>;

namespace english {

    const KeyCharMap lowercase{ 0,
        // ALLEGRO_KEY_A ... ALLEGRO_KEY_Z
        'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j',
        'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't',
        'u', 'v', 'w', 'x', 'y', 'z',
        // ALLEGRO_KEY_0 ... ALLEGRO_KEY_9
        '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
        // ALLEGRO_KEY_PAD_0 ... ALLEGRO_KEY_PAD_9
        '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
        // ALLEGRO_KEY_F1 ... ALLEGRO_KEY_F12
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        // ALLEGRO_KEY_ESC ... ALLEGRO_KEY_COMMA
        0, '`', '-', '=', 0, 0, '[', ']', '\n', ';', '\'',
        // ALLEGRO_KEY_BACKSLASH ... ALLEGRO_KEY_LEFT
        '\\', '\\', ',', '.', '/', ' ', 0, 0, 0, 0, 0, 0, 0,
        // ALLEGRO_KEY_RIGHT ... ALLEGRO_KEY_KANA
        0, 0, 0, '/', '*', '-', '+', 0, '\n', 0, 0, 0, 0, 0,
        // ALLEGRO_KEY_CONVERT ... ALLEGRO_KEY_KANA
        0, 0, 0, 0, ';', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        // ALLEGRO_KEY_CONVERT ... ALLEGRO_KEY_NUMLOCK
        0, 0, 0, 0, ';', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        // ALLEGRO_KEY_CAPSLOCK ... ALLEGRO_KEY_COMMAND
        0, '=', '`', ';', 0 };
        
    const KeyCharMap uppercase{ 0,
        // ALLEGRO_KEY_A ... ALLEGRO_KEY_Z
        'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J',
        'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T',
        'U', 'V', 'W', 'X', 'Y', 'Z',
        // ALLEGRO_KEY_0 ... ALLEGRO_KEY_9
        ')', '!', '@', '#', '$', '%', '^', '&', '*', '(',
        // ALLEGRO_KEY_PAD_0 ... ALLEGRO_KEY_PAD_9
        '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
        // ALLEGRO_KEY_F1 ... ALLEGRO_KEY_F12
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        // ALLEGRO_KEY_ESC ... ALLEGRO_KEY_COMMA
        0, '~', '_', '+', 0, 0, '{', '}', '\n', ':', '\"',
        // ALLEGRO_KEY_BACKSLASH ... ALLEGRO_KEY_LEFT
        '|', '|', '<', '>', '?', ' ', 0, 0, 0, 0, 0, 0, 0,
        // ALLEGRO_KEY_RIGHT ... ALLEGRO_KEY_KANA
        0, 0, 0, '/', '*', '-', '+', 0, '\n', 0, 0, 0, 0, 0,
        // ALLEGRO_KEY_CONVERT ... ALLEGRO_KEY_KANA
        0, 0, 0, 0, ':', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        // ALLEGRO_KEY_CONVERT ... ALLEGRO_KEY_NUMLOCK
        0, 0, 0, 0, ':', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        // ALLEGRO_KEY_CAPSLOCK ... ALLEGRO_KEY_COMMAND
        0, '+', '~', ':', 0 };

} // namespace english

class Keyboard {
private:

    std::bitset<ALLEGRO_KEY_MAX> is_pressed_, has_changed_;

public:

    const ALLEGRO_DISPLAY *const display; // non-owning

    static const char *get_key_name(int keycode) {
        assert(keycode > 0 && keycode < ALLEGRO_KEY_MAX && "[Keyboard] invalid keycode passed");
        return al_keycode_to_name(keycode);
    }

    using Poll = std::bitset<ALLEGRO_KEY_MAX>;

    Keyboard(ALLEGRO_DISPLAY *display)
        : display(display), is_pressed_(), has_changed_()
    {
        assert(al_is_keyboard_installed() && "[Keyboard] keyboard driver is not installed");
        assert(display != nullptr && "[Mouse] display cannot be null");
    }

    bool handle_event(const ALLEGRO_EVENT &event) {
        bool is_handled = true;
        switch(event.type) {
        case ALLEGRO_EVENT_KEY_DOWN:
            is_pressed_[event.keyboard.keycode] = true;
            has_changed_[event.keyboard.keycode] = true;
            break;
        case ALLEGRO_EVENT_KEY_UP:
            is_pressed_[event.keyboard.keycode] = false;
            has_changed_[event.keyboard.keycode] = true;
            break;
        default:
            is_handled = false;
            break;
        }
        return is_handled;
    }
    
    Poll poll() {
        Poll status;
        for (int i = 0; i < ALLEGRO_KEY_MAX; i++) {
            status[i] = is_pressed_[i] || has_changed_[i];
            has_changed_[i] = false;
        }
        return status;
    }

    bool is_key_pressed(int keycode) const {
        assert(keycode > 0 && keycode < ALLEGRO_KEY_MAX && "[Keyboard] invalid keycode passed");
        return is_pressed_[keycode];
    }
    
};

