#pragma once
#include "entity.hh"
#include "vector2.hh"

struct Action {

    virtual void step(Entity &entity) = 0;
    virtual bool is_complete() const = 0;

};

struct TimedAction : Action {

    TimedAction(unsigned duration) : duration_(duration), step_(0u) {}

    bool is_complete() const override { return step_ >= duration_; }
    void step(Entity &entity) override { step_++; advance(entity); }

    virtual void advance(Entity &entity) = 0;
    unsigned step_number() const { return step_; }
    unsigned duration() const { return duration_; }

private:

    unsigned duration_, step_;

};

struct MoveAction : TimedAction {

    MoveAction(Vector2<float> from, Vector2<float> to, int duration)
        : TimedAction(duration), from_(from), to_(to)
    { }
    
    void advance(Entity &entity) override {
        if (is_complete()) entity.position = to_;
        else entity.position = from_ + (to_ - from_) * float(step_number()) / float(duration());
    }
    
    Vector2<float> from() const { return from_; }
    Vector2<float> to()   const { return to_; }

private:

    Vector2<float> from_, to_;

};

