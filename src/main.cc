#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <iostream>

#include "display.hh"
#include "eventqueue.hh"
#include "font.hh"
#include "layout.hh"
#include "string.hh"
#include "text.hh"

void must_init(bool test, const char *error_message) {
    if (test) return;
    std::cerr << error_message << '\n';
    exit(1);
}

/*
Font load_font(std::string name, std::string type, int size, int flags) {
    al_load_font(name + '-' + type, size, flags);
}
*/

int main(int argc, char **argv) {
    (void)argc; (void)argv; // prevent unused variable compiler warning
    
    std::string font_path = "assets/font/";
    int font_size = 16;

    if (argc > 1) font_path += argv[1];
    else font_path += "PressStart2P-Regular.ttf";
    if (argc > 2) font_size = std::stoi(argv[2]);

    std::cout << "font path: " << font_path << '\n';
    std::cout << "font size: " << font_size << '\n';

    must_init(al_init(), "Failed to initialize allegro.");
    must_init(al_install_keyboard(), "Failed to install keyboard driver.");
    must_init(al_install_mouse(), "Failed to install mouse driver.");
    must_init(al_init_font_addon(), "Failed to initialize font addon.");
    must_init(al_init_ttf_addon(), "Failed to initialize ttf addon.");
    must_init(al_init_primitives_addon(), "Failed to initialize primitives addon.");

    Display display(640, 480, "Hello World"); // create display before loading fonts
    must_init(display, "Failed to create display.");

    EventQueue event_queue{
        al_get_keyboard_event_source(),
        al_get_mouse_event_source(),
        display };
    must_init(event_queue, "Failed to create event queue.");

    const char *example_text = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Nam sit amet mauris quis tortor consequat maximus a sit amet est. Aliquam ullamcorper lorem eu nunc ultrices sollicitudin. Quisque est nunc, scelerisque pellentesque luctus at, ultricies in lorem. Nam malesuada commodo suscipit. Donec rhoncus, ipsum a condimentum gravida, mauris mauris facilisis magna, id interdum metus felis eget libero. Quisque condimentum risus ut elit consequat semper. Mauris ultrices gravida nibh, a sollicitudin lorem iaculis sed. Integer fringilla accumsan lorem, a rutrum orci blandit sit amet. Donec ultrices libero a ante accumsan, vel semper orci posuere. Curabitur eget urna imperdiet, vulputate neque dapibus, pellentesque nisl. Ut felis mi, ultrices non dui nec, rhoncus efficitur est.";

    Font font(font_path.data(), font_size);
    must_init(font, "Failed to load font.");

    auto window = GridLayout(display.area(), 6, 4, {20.f, 20.f});

    Text title(window.area(1, 0, 4, 1), font, "snow", "Hello World!", ALIGN_CENTER);
    Text body(window.area(1, 1, 4, 2), font, "snow", example_text, ALIGN_CENTER, 3);

    title.per_char_cb = rainbow_text_effect;
    body.per_char_cb = wavy_text_effect;

    ALLEGRO_EVENT event;
    bool play = true;
    double time_prior = al_get_time();
    // double lag = 0.0, step_size = 1.0 / 60.0;

    //// MAIN LOOP
    while (play) {
        double time_current = al_get_time(),
               time_elapsed = time_current - time_prior;

        /// HANDLE EVENTS
        while (event_queue.next(event)) {
            display.handle_event(event);
            switch (event.type) {
            case ALLEGRO_EVENT_KEY_DOWN:
                play = event.keyboard.keycode != ALLEGRO_KEY_ESCAPE;
                break;
            case ALLEGRO_EVENT_DISPLAY_CLOSE:
                play = false;
                break;
            }
        }

        /// UPDATE
        // play = update(time_elapsed);
        progress_animations();

        /// FIXED UPDATE
        // for (lag += dt; lag >= step_size; lag -= step_size) fixed_update();

        /// RENDER
        al_clear_to_color(al_map_rgb(0,0,0));
        title.area().draw_outline("snow", 2.0f);
        body.area().draw_outline("snow", 2.0f);
        title.render();
        body.render();
        al_flip_display();
        
        time_prior = time_current;
    }

    return 0;
}

