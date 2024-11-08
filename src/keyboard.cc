#include "keyboard.hh"

KeyboardLog::KeyboardLog(ALLEGRO_DISPLAY *display)
    : display(display), is_pressed_(), has_changed_()
{
    assert(al_is_keyboard_installed() && "[Keyboard] keyboard driver is not installed");
    assert(display != nullptr && "[Mouse] display cannot be null");
}

bool KeyboardLog::handle_event(const ALLEGRO_EVENT &event) {
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

std::bitset<ALLEGRO_KEY_MAX> KeyboardLog::poll() {
    std::bitset<ALLEGRO_KEY_MAX> status;
    for (int i = 0; i < ALLEGRO_KEY_MAX; i++) {
        status[i] = is_pressed_.test(i) || has_changed_.test(i);
        has_changed_.reset(i);
    }
    return status;
}

