#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <iostream>

#include "camera.hh"
#include "display.hh"
#include "event_queue.hh"
#include "font.hh"
#include "keyboard.hh"
#include "mouse.hh"

void must_init(bool test, const char *error_message) {
    if (test) return;
    std::cerr << error_message << '\n';
    exit(1);
}

int main(int argc, char **argv) {
    // must have argc and argv variables, required by allegro 
    (void)argc; (void)argv; // prevent unused variable compiler warning

    must_init(al_init(), "Failed to initialize allegro.");
    must_init(al_install_keyboard(), "Failed to install keyboard driver.");
    must_init(al_install_mouse(), "Failed to install mouse driver.");
    must_init(al_init_font_addon(), "Failed to initialize font addon.");
    must_init(al_init_ttf_addon(), "Failed to initialize ttf addon.");

    al_set_new_display_flags(ALLEGRO_RESIZABLE | ALLEGRO_WINDOWED);
    Display display{640, 480, "wip"};
    Keyboard keyboard(display.get());
    Mouse mouse(display.get());
    Camera camera({0,0,0}, display.get_width(), display.get_height());
    Font font{"assets/font/PressStart2P-Regular.ttf", 36};
    EventQueue event_queue{
        al_get_keyboard_event_source(),
        al_get_mouse_event_source(),
        display.get_event_source() };
    ALLEGRO_EVENT event;
    bool play = true, update = true, render = true;

    while (play) {
        // handle event
        while (!event_queue.is_empty()) {
            event_queue.wait_for_event(event);
            display.handle_event(event);
            mouse.handle_event(event);
            keyboard.handle_event(event);
            switch (event.type) {
            case ALLEGRO_EVENT_DISPLAY_CLOSE:
                if (event.display.source == display) {
                    play = false;
                } break;
            case ALLEGRO_EVENT_DISPLAY_RESIZE:
                if (event.display.source == display) {
                    camera.viewport.width = display.get_width();
                    camera.viewport.height = display.get_height();
                } break;
            }
        }

        if (update) {
            // handle input
            // world.clear();
            auto keyboard_input = keyboard.poll();
            if (keyboard_input[ALLEGRO_KEY_ESCAPE]) play = false;
            // if (keyboard_input[ALLEGRO_KEY_UP]) player.coord.y++;
            // if (keyboard_input[ALLEGRO_KEY_DOWN]) player.coord.y--;
            // if (keyboard_input[ALLEGRO_KEY_RIGHT]) player.coord.x++;
            // if (keyboard_input[ALLEGRO_KEY_LEFT]) player.coord.x--;
            
            // update
            // update = false;
        }
        
        // render
        if (render) {
            al_set_target_backbuffer(display.get());
            al_clear_to_color(al_map_rgb(0,0,0));
            al_draw_text(font.get(), al_map_rgb(255,255,255),
                    display.get_width() / 2.0f, display.get_height() / 2.0f,
                    ALLEGRO_ALIGN_CENTRE, "HELLO WORLD"); 
            /*
            for (auto coord : camera.range()) {
                coord.x = coord.x % world.width();
                coord.y = coord.y % world.height();
                for (auto id : world[coord])
                    render(camera.translate(coord), id);
            }
            */
            al_flip_display();
            // render = false;
        }
    }

    return 0;
}
