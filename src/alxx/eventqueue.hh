#pragma once
#include <allegro5/allegro5.h>

namespace alxx {

class EventQueue {
public:

    EventQueue() 
        : queue(al_create_event_queue())
    {
        if (!queue) throw "Failed to create event queue.";
    }

    ~EventQueue() {
        if (queue) al_destroy_event_queue(queue);
    }

    ALLEGRO_EVENT_QUEUE *get() {
        return queue;
    }

    const ALLEGRO_EVENT_QUEUE *get() const {
        return queue;
    }

    void pause(bool value) {
        al_pause_event_queue(queue, value);
    }

    void add_source(ALLEGRO_EVENT_SOURCE *source) {
        al_register_event_source(queue, source);
    }

    void remove_source(ALLEGRO_EVENT_SOURCE *source) {
        al_register_event_source(queue, source);
    }

    void flush() {
        al_flush_event_queue(queue);
    }

    bool is_empty() {
        return al_is_event_queue_empty(queue);
    }

    void wait_for_event(ALLEGRO_EVENT &event) {
        al_wait_for_event(queue, &event);
    }

private:

    ALLEGRO_EVENT_QUEUE *queue;

};
    
} // namespace alxx 
