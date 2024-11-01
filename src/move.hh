#pragma once
#include "vector2.hh"
//#include <cstdint>
//#include <iostream>

/*
const Vector2<float>
    EAST{1.f,0.f},  WEST{-1.f,0.f},
    NORTH{0.f,1.f}, SOUTH{0.f,-1.f};

enum Direction : uint8_t {
    EAST, WEST,
    NORTH, SOUTH,
    NORTHEAST, NORTHWEST,
    SOUTHEAST, SOUTHWEST
};
*/

struct MoveComponent {

    MoveComponent()
        : from_{0.f,0.f}, to_{0.f,0.f}, duration_{0u}, progress_{0u}
    {}

    MoveComponent(Vector2<float> from, Vector2<float> to, int steps)
        : from_(from), to_(to), duration_(steps), progress_(0u)
    {}
    
    bool active() const { return progress_ < duration_; }

    Vector2<float> step() {
        progress_++;
        //printf("[Move::step] %i/%i\n", progress_, duration_);
        return active() ? from_ + (to_ - from_) * float(progress_) / float(duration_) : to_;
    }
    
    int progress() const { return progress_; }
    int duration() const { return duration_; }
    Vector2<float> from() const { return from_; }
    Vector2<float> to() const { return to_; }

    void activate(Vector2<float> from, Vector2<float> to, unsigned steps) {
        //printf("[Move::activate] {%f,%f} to {%f,%f} in %i steps\n", from.x, from.y, to.x, to.y, steps);
        from_ = from;
        /*
        to_ = from_;
        switch (Direction) {
        case NORTH:     to_ += {0.f, 1.f}; break;
        case SOUTH:     to_ += {0.f, -1.f}; break;
        case EAST:      to_ += {1.f, 0.f}; break;
        case WEST:      to_ += {-1.f, 0.f}; break;
        case NORTHEAST: to_ += {1.f, 1.f}; break;
        case NORTHWEST: to_ += {-1.f, 1.f}; break;
        case SOUTHEAST: to_ += {1.f, -1.f}; break;
        case SOUTHWEST: to_ += {-1.f, -1.f}; break;
        };
        */
        to_ = to;
        duration_ = steps;
        progress_ = 0u;
    }

private:

    Vector2<float> from_, to_;
    unsigned duration_, progress_;

};

