#pragma once
#include <allegro5/allegro.h>
#include <cassert>

// Wrapper for ALLEGRO_TIMER
struct Timer {

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

    operator ALLEGRO_EVENT_SOURCE *() const { return al_get_timer_event_source(timer_); }
    operator ALLEGRO_TIMER *() { return timer_; }
    operator const ALLEGRO_TIMER *() const { return timer_; }

private:

    ALLEGRO_TIMER *const timer_;

};
