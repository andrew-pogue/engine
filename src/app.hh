#pragma once
#include "alxx/core.hh"
#include "textbox.hh"
#include "keyboard.hh"

class App {
public:

    App(float, float);
    ~App();
    
    void run();

private:

    bool handle_event(const ALLEGRO_EVENT &);
    void render();
    void update();
    void fixed_update();

    alxx::Display display;
    alxx::EventQueue event_queue;
    alxx::Timer fixed_update_timer;
    double time_stamp, frame_rate;
    TextBox textbox, fps_counter;
    Keyboard keyboard;

};