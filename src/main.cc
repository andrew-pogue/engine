#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <iostream>

#include "app.hh"

void must_init(bool test, const char *error_message) {
    if (!test) {
        std::cerr << error_message << '\n';
        exit(1);
    }
}

int main(int argc, char** argv) {

    // Initialize Allegro:
    must_init(al_init(), "Failed to initialize allegro.");
    must_init(al_init_font_addon(), "Failed to initialize allegro font addon.");
    must_init(al_init_primitives_addon(), "Failed to initialize allegro primitives addon.");
    must_init(al_install_keyboard(), "Failed to install keyboard.");

    al_set_new_display_flags(ALLEGRO_RESIZABLE | ALLEGRO_WINDOWED);

    try {
        App app(640.0f, 400.0f);
        app.run();
    }

    catch (const char *e) {
        std::cerr << e << '\n';
    }

    return 0;

}
