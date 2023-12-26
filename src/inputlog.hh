#pragma once

#include <array>
#include <bitset>

namespace {
    const unsigned char
        ACTIVE  = 1,
        NEW = 2;
}

/** Chars in this array should only be one of these 4 values:
 * 00000011 ----> active, new       "just pressed"
 * 00000010 ----> deactive, new     "just released"
 * 00000001 ----> active, old       "remained pressed"
 * 00000000 ----> deactive, old     "remained released"
 * This method of doing things accounts for the occassion where
 * a key is pressed and released in-between polls (00000010).
 **/
enum : const unsigned char {
    KEY_JUST_PRESSED = ACTIVE | NEW,
    KEY_JUST_RELEASED = NEW,
    KEY_REMAINED_PRESSED = ACTIVE,
    KEY_REMAINED_RELEASED = 0
};

// A simple array that records changes made to the state of an input device
// (i.e. keyboard, mouse) over a length of time. SIZE is the number of buttons
// on the input device. It is assumed each button has a keycode ranging from
// [0] to [SIZE-1].
template <size_t SIZE>
class InputLog {
public:

    using Poll = std::array<unsigned char, SIZE>;

    class InputHandler {

        virtual void handle_input(const std::bitset<SIZE> &) = 0;

    };

    // Returns a bitset representing the status of each button (active = 1, deactive = 0).
    // A button is 'active' when its state was changed since the last poll.
    Poll poll() {
        Poll buttons = status_;
        for (size_t i = 0; i < SIZE; i++)
            status_[i] &= ~NEW;
        return buttons;
    }

    void press(int keycode) {
        status_.at(keycode) = ACTIVE | NEW;
    }

    void release(int keycode) {
        status_.at(keycode) &= ~ACTIVE;
    }

private:

    Poll status_;

};
