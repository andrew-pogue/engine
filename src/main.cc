#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <iostream>

#include "camera.hh"
#include "display.hh"
#include "eventqueue.hh"
#include "font.hh"
#include "keyboard.hh"
#include "mouse.hh"
#include "textbox.hh"

void must_init(bool test, const char *error_message) {
    if (test) return;
    std::cerr << error_message << '\n';
    exit(1);
}

int main(int argc, char **argv) {
    (void)argc; (void)argv; // prevent unused variable compiler warning

    must_init(al_init(), "Failed to initialize allegro.");
    must_init(al_install_keyboard(), "Failed to install keyboard driver.");
    must_init(al_install_mouse(), "Failed to install mouse driver.");
    must_init(al_init_font_addon(), "Failed to initialize font addon.");
    must_init(al_init_ttf_addon(), "Failed to initialize ttf addon.");
    must_init(al_init_primitives_addon(), "Failed to initialize primitives addon.");

    Display display(640, 480, ALLEGRO_RESIZABLE | ALLEGRO_WINDOWED, "wip");
    Keyboard keyboard(display.get());
    Mouse mouse(display.get());
    Camera camera({0,0,0}, display.get_width(), display.get_height());
    Font font{"assets/font/PressStart2P-Regular.ttf", 36};
    EventQueue event_queue{
        al_get_keyboard_event_source(),
        al_get_mouse_event_source(),
        display.get_event_source() };

    TextBox textbox(display.get(), 
        display.get_width() / 2.0f, display.get_height() / 2.0f,
        500.0f, 100.0f, "HELLO WORLD BONJOUR LE MONDE", font, al_map_rgb(0,0,0));
    textbox.border.enabled = true;
    textbox.border.thickness = 2.0f;
    textbox.border.color = al_map_rgb(0,155,0);
    textbox.border.offset = 4.0f;
    textbox.fill.enabled = true;
    textbox.fill.color = al_map_rgb(0,155,0);
    textbox.padding = 4.0f;
    textbox.align.vertical = Align::CENTER;
    textbox.align.horizontal = Align::CENTER;
    textbox.text.flags = ALLEGRO_ALIGN_CENTRE;

    ALLEGRO_EVENT event;
    const double step_size = 1.0/60.0;
    bool play = true, render = true;
    double lag = 0.0, time_prior = al_get_time();

    while (play) {
        double time_current = al_get_time(),
               time_elapsed = time_current - time_prior;

        ///// HANDLE EVENT /////////////////////////////////////////////////
        
        while (event_queue.next(event)) {
            display.handle_event(event);
            mouse.handle_event(event);
            keyboard.handle_event(event);
            textbox.handle_event(event);
            switch (event.type) {
            case ALLEGRO_EVENT_DISPLAY_CLOSE:
                if (event.display.source == display) {
                    play = false;
                } break;
            case ALLEGRO_EVENT_DISPLAY_RESIZE:
                if (event.display.source == display) {
                    textbox.x = display.get_width() / 2.0f;
                    textbox.y = display.get_height() / 2.0f;
                    camera.viewport.width = display.get_width();
                    camera.viewport.height = display.get_height();
                } break;
            }
        }

        ///// HANDLE INPUT /////////////////////////////////////////////////
        
        auto keyboard_input = keyboard.poll();
        if (keyboard_input[ALLEGRO_KEY_ESCAPE]) play = false;
        
        ///// FIXED UPDATE /////////////////////////////////////////////////
        /// fixed time step for physics and game logic
        
        for (lag += time_elapsed; lag >= step_size; lag -= step_size) {
        }
        
        ///// UPDATE //////////////////////////////////////////////////////
        /// variable time step for gui and visual effects

        ///// RENDER //////////////////////////////////////////////////////
        
        al_set_target_backbuffer(display.get());
        al_clear_to_color(al_map_rgb(0,0,0));
        textbox.render();
        /*
        for (auto coord : camera.range()) {
            coord.x = coord.x % world.width();
            coord.y = coord.y % world.height();
            for (auto id : world[coord])
                render(camera.translate(coord), id);
        }
        */
        al_flip_display();
        
        //////////////////////////////////////////////////////////////////

        time_prior = time_current;
    }

    return 0;
}
