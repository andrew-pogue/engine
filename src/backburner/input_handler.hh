#pragma once

#include <array>
#include <functional>
#include <vector>

template <size_t BUTTON_MAX>
class InputHandler {
public:

    using command_t = std::function<void(size_t)>;

    InputHandler()
        : status_()
        , commands_()
    {
        status_.fill(0);
    }

    void press(int keycode) {
        key_status_.at(keycode) = ACTIVE | NEW;
    }

    void release(int keycode) {
        key_status_.at(keycode) &= ~ACTIVE;
    }

    // Run the command of every button whose status is active.
    void update() {
        for (size_t button = 0; button < status_.size(); button++) {
            if (status_[button]) {
                status_[button] &= ~NEW;
                if (commands_[button]) {
                    commands_[button](button);
                } else if (on_default) {
                    on_default(button);
                }
            }
        }
    }

    // Gets every button whose status is active.
    auto poll() const {
        std::vector<size_t> input;
        input.reserve(BUTTON_MAX);
        for (size_t button = 0; button < status_.size(); button++) {
            if (status_[button]) {
                input.push_front(button);
            }
        }
        input.shrink_to_fit();
        return input;
    }

    // Returns true if the button was pressed since the last update.
    // If the button ID is invalid, throws an std::out_of_range exception.
    bool is_active(size_t button) const {
        return status_.at(button);
    }

    // Gets the command associated with the given button ID.
    // If the button ID is invalid, throws an std::out_of_range exception.
    command_t &on_input(size_t button) {
        return commands_.at(button);
    }

    size_t size() const {
        return BUTTON_MAX;
    }

    command_t on_default;

private:

    static const unsigned char
        ACTIVE  = 1,
        NEW = 2;

    /** Chars in this array should only be one of these 4 values:
     * 00000011 ----> active, new       "pressed"
     * 00000001 ----> active, old       "remained pressed"
     * 00000010 ----> deactive, new     "pressed and released"
     * 00000000 ----> deactive, old     "remained released"
     * This method of doing things accounts for the occassion where
     * a key is pressed and released in-between updates (00000010).
     **/
    std::array<unsigned char, BUTTON_MAX> status_;

    std::array<command_t, BUTTON_MAX> commands_;

};





// std::array<unsigned, BUTTON_MAX> delay_;
// std::array<unsigned, BUTTON_MAX> cooldown_;

// ignore this bullshit, take the simple route, have an on_update functor that you call
// on_update passes the poll and processes it
// nope then can't configure keycommands



/*
struct Controls
{
    Key m_MovementUp;
    Key m_MovementDown;
    Key m_MovementLeft;
    Key m_MovementRight;

    Key m_Shoot;
}
*/


// return std::array<bool, BUTTON_MAX>(status_);




// rather than save function in key
// save the key in the function
// inputhandler is passed the output of a poll
// order of which input is processed isn't dont like htis shit up here
