#pragma once
#include <allegro5/allegro.h>
#include <cassert>

// Wrapper for ALLEGRO_TIMER
class Timer {
private:

    ALLEGRO_TIMER *const timer_;

public:

    Timer(const Timer &) = delete; 
    void operator=(const Timer &) = delete; 

    Timer(double seconds) 
        : timer_(al_create_timer(seconds))
    { assert(timer_ && "failed to create timer"); }
    
    Timer(Timer &&other)
        : timer_(other.timer_)
    { assert(timer_ && "failed to create timer"); }

    ~Timer() { al_destroy_timer(timer_); }

    void start() { al_start_timer(timer_); }
    void stop() { al_stop_timer(timer_); }
    void resume() { al_resume_timer(timer_); }

    bool operator==(const ALLEGRO_TIMER *timer) const { return timer == timer_; }
    bool operator!=(const ALLEGRO_TIMER *timer) const { return timer != timer_; }

    ALLEGRO_EVENT_SOURCE *get_event_source() const { return al_get_timer_event_source(timer_); }
    ALLEGRO_TIMER *get() { return timer_; }

};
