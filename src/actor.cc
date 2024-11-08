#include "actor.hh"

void Actor::step() {
    if (action_) {
        action_->step(*this);
        if (action_->is_complete()) {
            delete action_;
            action_ = nullptr;
        }
    }
}

bool Actor::move(uint8_t direction) {
    if (action_) return false;
    float x = position.x + float(bool(direction & EAST)) - float(bool(direction & WEST)),
          y = position.y + float(bool(direction & NORTH)) - float(bool(direction & SOUTH));
    //x += float(x < 0) * terrain.width  - float(x >= terrain.width)  * terrain.width;
    //y += float(y < 0) * terrain.height - float(y >= terrain.height) * terrain.height;
    if (position.x == x && position.y == y) return true;
    action_ = new MoveAction(position, {x, y}, 10);
    return action_;
}

