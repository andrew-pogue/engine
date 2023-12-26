#pragma once
#include <allegro5/allegro5.h>

namespace alxx {

class Timer {
public:

    Timer(double seconds)
        : timer(al_create_timer(seconds))
    {
        if (!timer) throw "Failed to create timer.";
    }

    ~Timer() {
        if (timer) al_destroy_timer(timer);
    }

    auto get_event_source() {
        return al_get_timer_event_source(timer);
    }

    void stop() {
        al_stop_timer(timer);
    }

    void start() {
        al_start_timer(timer);
    }

private:

    ALLEGRO_TIMER *timer;

};

} // namespace alxx