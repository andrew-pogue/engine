#include <allegro5/allegro.h>
#include <allegro5/allegro_color.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <iostream>

#include "layout.hh"
#include "text.hh"
#include "text-effect.hh"

const int DISPLAY_WIDTH = 640, DISPLAY_HEIGHT = 480;

void must_init(bool test, const char *error_message) {
    if (test) return;
    std::cerr << error_message << '\n';
    exit(1);
}

void draw_wave(int x1, int x2, int y, ALLEGRO_COLOR color, float wave_length, float wave_height, float shift) {
    for (int i = x1; i < x2; i++) {
        int j = y + sinf(2.f * PI / wave_length * (i + shift)) * wave_height / 2.f;
        al_put_pixel(i, j-1, color);
        al_put_pixel(i, j, color);
        al_put_pixel(i, j+1, color);
    }
}

void draw_ui(const ALLEGRO_FONT *font, double time) {
    static const char *example_text = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Nam sit amet mauris quis tortor consequat maximus a sit amet est. Aliquam ullamcorper lorem eu nunc ultrices sollicitudin. Quisque est nunc, scelerisque pellentesque luctus at, ultricies in lorem. Nam malesuada commodo suscipit. Donec rhoncus, ipsum a condimentum gravida, mauris mauris facilisis magna, id interdum metus felis eget libero. Quisque condimentum risus ut elit consequat semper. Mauris ultrices gravida nibh, a sollicitudin lorem iaculis sed. Integer fringilla accumsan lorem, a rutrum orci blandit sit amet. Donec ultrices libero a ante accumsan, vel semper orci posuere. Curabitur eget urna imperdiet, vulputate neque dapibus, pellentesque nisl. Ut felis mi, ultrices non dui nec, rhoncus efficitur est.";

    GridLayout window({0.f, 0.f, float(DISPLAY_WIDTH), float(DISPLAY_HEIGHT)}, 6, 4, {20.f, 20.f});

    // draw_animated_text(...effect(dt...))
    // draw_text_with_effect(...)
    // WavyTextEffect(2.f,2.f,PI/2.f*time*10.f)
    
    draw_wave(0, DISPLAY_WIDTH, DISPLAY_HEIGHT-20.f, al_color_name("blue"), 88.f, 10.f, time);

    auto title_area = window.area(1, 0, 4, 1);
    draw_text_with_effect(title_area.x, title_area.y, font, al_color_name("snow"), "HELLO WORLD!",
        std::bind_front(RainbowTextEffect(time), 0), ALIGN_CENTER);
    draw_multiline_text_with_effect(window.area(1, 1, 4, 2), font, al_color_name("snow"), example_text,
        WavyTextEffect(88.f,10.f,time/2.0), ALIGN_CENTER);
}

ALLEGRO_FONT *load_font(int argc, char **argv) {
    const char *name = (argc > 1) ? argv[1] : "Inconsolata";
    const char *style = (argc > 2) ? argv[2] : "Regular";
    int size = (argc > 3) ? std::stoi(argv[3]) : 16;
    std::cout << "font: " << name << ' ' << style << ' ' << size << '\n';
    std::string path = "assets/font/";
    path += name;
    path += '-';
    path += style;
    path += ".ttf";
    return al_load_ttf_font(path.data(), size, 0);
}

int main(int argc, char **argv) {
    (void)argc; (void)argv; // prevent unused variable compiler warning
    
    must_init(al_init(), "Failed to initialize allegro.");
    must_init(al_install_keyboard(), "Failed to install keyboard driver.");
    must_init(al_install_mouse(), "Failed to install mouse driver.");
    must_init(al_init_font_addon(), "Failed to initialize font addon.");
    must_init(al_init_ttf_addon(), "Failed to initialize ttf addon.");
    must_init(al_init_primitives_addon(), "Failed to initialize primitives addon.");

    al_set_new_window_title("Hello World");
    ALLEGRO_DISPLAY *display = al_create_display(DISPLAY_WIDTH, DISPLAY_HEIGHT); 
    must_init(display, "Failed to create display.");

    ALLEGRO_EVENT_QUEUE *event_queue = al_create_event_queue();
    must_init(event_queue, "Failed to create event queue.");
    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_register_event_source(event_queue, al_get_mouse_event_source());

        // NOTE: you must create a display before loading a font or allegro will be bugged
    ALLEGRO_FONT *font = load_font(argc, argv);
    must_init(font, "Failed to load font.");
    
    ALLEGRO_EVENT event;
    bool play = true;
    double time_prior = al_get_time();
    // double lag = 0.0, step_size = 1.0 / 60.0;

    /// MAIN LOOP
    while (play) {
        double time_current = al_get_time(),
               time_elapsed = time_current - time_prior;

        /// HANDLE EVENTS
        while (al_get_next_event(event_queue, &event)) {
            switch (event.type) {
            case ALLEGRO_EVENT_KEY_DOWN:
                play = event.keyboard.keycode != ALLEGRO_KEY_ESCAPE;
                break;
            case ALLEGRO_EVENT_DISPLAY_CLOSE:
                play = false;
                break;
            /*
            case ALLEGRO_EVENT_DISPLAY_RESIZE:
                if (event.display.source == display) {
                    al_acknowledge_resize(display);
                } break;
            case ALLEGRO_EVENT_DISPLAY_SWITCH_OUT:
                if (event.display.source == display) {
                    if (al_is_keyboard_installed()) al_clear_keyboard_state(display);
                } break;
            */
            }
        }

        /// UPDATE
        // play = update(time_elapsed);

        /// FIXED UPDATE
        // for (lag += dt; lag >= step_size; lag -= step_size) fixed_update();

        /// RENDER
        al_clear_to_color(al_map_rgb(0,0,0));
        draw_ui(font, time_current * 100);
        al_flip_display();
        
        time_prior = time_current;
    }

    al_destroy_font(font);
    al_destroy_event_queue(event_queue);
    al_destroy_display(display);

    return 0;
}

