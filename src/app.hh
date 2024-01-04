#pragma once
#include "alxx/core.hh"
#include "textbox.hh"
#include "rectangle.hh"
#include "keyboard.hh"

class App {
public:

    App(float width, float height, double fixed_time_step);
    ~App();
    
    void run();

private:

    bool handle_event(const ALLEGRO_EVENT &event);
    void render();
    // variable time step
    bool update(double delta_time);
    // fixed time step
    bool fixed_update();

    const double fixed_time_step;
    alxx::Display display;
    alxx::EventQueue event_queue;
    // alxx::Timer fixed_update_timer;
    TextBox textbox, fps_counter;
    Rectangle rectangle;
    float x, y;
    Keyboard keyboard;

};