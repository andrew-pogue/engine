#pragma once
#include <cstdint>
#include "action.hh"
#include "entity.hh"

const uint8_t
    EAST      = 0b0001,
    WEST      = 0b0010,
    NORTH     = 0b0100,
    SOUTH     = 0b1000;

struct Actor : Entity {

    Actor(Vector2<float> position) : Entity(position), action_(nullptr) {}
    virtual ~Actor() { delete action_; }

    bool busy() const { return action_; }

    void step();
    bool move(uint8_t direction);

protected:

    Action *action_;

};

