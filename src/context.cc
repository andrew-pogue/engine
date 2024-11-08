#include "context.hh"

PlayerContext::PlayerContext(ALLEGRO_DISPLAY *display, Actor *player) 
    : player(player)
    , keycommands(), keyboard_(display)
    , repeat_delay_(0.0)
{ assert(player); }

bool PlayerContext::update(double dt) {
    bool status = !keyboard_[ALLEGRO_KEY_ESCAPE];
    if (repeat_delay_ > 0.0) repeat_delay_ -= dt;
    if (!player || player->busy()) {
        keyboard_.clear_changes();
        return status;
    }

    // If movement key(s) pressed
    if (   keyboard_[keycommands.move_up]
        || keyboard_[keycommands.move_down]
        || keyboard_[keycommands.move_left]
        || keyboard_[keycommands.move_right]
    ) {
        bool is_auto_repeat = 
               !keyboard_.has_changed(keycommands.move_up)
            && !keyboard_.has_changed(keycommands.move_down)
            && !keyboard_.has_changed(keycommands.move_left)
            && !keyboard_.has_changed(keycommands.move_right);
        if (!is_auto_repeat || repeat_delay_ <= 0.0) {
            std::uint8_t direction = 0u
                | (NORTH * keyboard_[keycommands.move_up])
                | (SOUTH * keyboard_[keycommands.move_down])
                | (WEST  * keyboard_[keycommands.move_left])
                | (EAST  * keyboard_[keycommands.move_right]);
            player->move(direction);
            repeat_delay_ = 0.2;
        }
    }
    keyboard_.clear_changes();
    return status;
}

