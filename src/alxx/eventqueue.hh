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

    // gets a copy of the event at the top of the queue
    // returns true if there is an event in queue
    // if queue is empty, value of event is unspecified
    bool peek(ALLEGRO_EVENT &event) {
        return al_peek_next_event(queue, &event);
    }

    // gets and removes the event at the top of the queue
    // returns true if there is an event in queue
    // if queue is empty, value of event is unspecified
    bool next(ALLEGRO_EVENT &event) {
        return al_get_next_event(queue, &event);
    }

    // while paused, new events are ignored
    // while unpaused, new events are pushed onto back of the queue
    void pause(bool value) {
        al_pause_event_queue(queue, value);
    }

    bool is_paused() const {
        return al_is_event_queue_paused(queue);
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

    bool is_empty() const {
        return al_is_event_queue_empty(queue);
    }

    // waits for an event if there are none in queue
    void wait() {
        al_wait_for_event(queue, NULL);
    }

    // gets and removes the event at the top of the queue
    // waits for an event if there are none in queue
    void wait(ALLEGRO_EVENT &event) {
        al_wait_for_event(queue, &event);
    }

    bool wait_until(ALLEGRO_TIMEOUT &timeout) {
        return al_wait_for_event_until(queue, NULL, &timeout);
    }

    bool wait_until(ALLEGRO_TIMEOUT &timeout, ALLEGRO_EVENT &event) {
        return al_wait_for_event_until(queue, &event, &timeout);
    }

    bool wait_timed(float seconds) {
        return al_wait_for_event_timed(queue, NULL, seconds);
    }

    bool wait_timed(float seconds, ALLEGRO_EVENT &event) {
        return al_wait_for_event_timed(queue, &event, seconds);
    }

private:

    ALLEGRO_EVENT_QUEUE *queue;

};
    
} // namespace alxx 
