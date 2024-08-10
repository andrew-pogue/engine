#pragma once
#include <allegro5/allegro.h>
#include <initializer_list>

// wrapper class for ALLEGRO_EVENT_QUEUE
class EventQueue {
private:

    ALLEGRO_EVENT_QUEUE *const queue_;

public:

    EventQueue(const EventQueue &) = delete;
    void operator=(const EventQueue &) = delete;
    EventQueue(EventQueue &&) = delete;
    void operator=(EventQueue &&) = delete;

    EventQueue()
        : queue_(al_create_event_queue())
    { assert(queue_ && "failed to create event queue"); }
    
    EventQueue(std::initializer_list<ALLEGRO_EVENT_SOURCE *> sources)
        : queue_(al_create_event_queue())
    {
        assert(queue_ && "failed to create event queue");
        add_event_sources(sources);
    }

    ~EventQueue() { al_destroy_event_queue(queue_); }

    bool operator==(ALLEGRO_EVENT_QUEUE *queue) const { return queue == queue_; }
    bool operator!=(ALLEGRO_EVENT_QUEUE *queue) const { return queue != queue_; }

    bool peek(ALLEGRO_EVENT &event) { return al_peek_next_event(queue_, &event); }
    bool next(ALLEGRO_EVENT &event) { return al_get_next_event(queue_, &event); }

    void pause(bool value) { al_pause_event_queue(queue_, value); }
    bool is_paused() const { return al_is_event_queue_paused(queue_); }

    void flush() { al_flush_event_queue(queue_); }

    void add_event_source(ALLEGRO_EVENT_SOURCE *source) { al_register_event_source(queue_, source); }
    void add_event_sources(std::initializer_list<ALLEGRO_EVENT_SOURCE *> sources) { for (auto src : sources) add_event_source(src); }
    void remove_event_source(ALLEGRO_EVENT_SOURCE *source) { al_unregister_event_source(queue_, source); }
    bool has_event_source(ALLEGRO_EVENT_SOURCE *source) { return al_is_event_source_registered(queue_, source); }

    bool is_empty() { return al_is_event_queue_empty(queue_); }

    void wait_for_event(ALLEGRO_EVENT &event) { al_wait_for_event(queue_, &event); }
    void wait_for_event() { al_wait_for_event(queue_, NULL); }

    bool wait_until(ALLEGRO_TIMEOUT &timeout) { return al_wait_for_event_until(queue_, NULL, &timeout); }
    bool wait_until(ALLEGRO_TIMEOUT &timeout, ALLEGRO_EVENT &event) { return al_wait_for_event_until(queue_, &event, &timeout); }
    bool wait_timed(float seconds) { return al_wait_for_event_timed(queue_, NULL, seconds); }
    bool wait_timed(float seconds, ALLEGRO_EVENT &event) { return al_wait_for_event_timed(queue_, &event, seconds); }

    ALLEGRO_EVENT_QUEUE *get() { return queue_; }

};
