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
    Font font_title{"assets/font/PressStart2P-Regular.ttf", 36};
    Font font_body{"assets/font/PressStart2P-Regular.ttf", 14};
    EventQueue event_queue{
        al_get_keyboard_event_source(),
        al_get_mouse_event_source(),
        display.get_event_source() };

    TextBox title(display.get(), 
        display.get_width() / 2.0f, 50.0f,
        500.0f, 50.0f, "HELLO WORLD", font_title, al_map_rgb(0,0,0));
    title.border.enabled = true;
    title.border.thickness = 2.0f;
    title.border.color = al_map_rgb(255,255,255);
    title.border.padding = 4.0f;
    title.fill.enabled = true;
    title.fill.color = al_map_rgb(0,155,0);
    title.align.vertical = Align::CENTER;
    title.align.horizontal = Align::CENTER;
    title.text.padding = 4.0f;
    title.text.flags = ALLEGRO_ALIGN_CENTRE;

    const char *txt = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Nam sit amet mauris quis tortor consequat maximus a sit amet est. Aliquam ullamcorper lorem eu nunc ultrices sollicitudin. Quisque est nunc, scelerisque pellentesque luctus at, ultricies in lorem. Nam malesuada commodo suscipit. Donec rhoncus, ipsum a condimentum gravida, mauris mauris facilisis magna, id interdum metus felis eget libero. Quisque condimentum risus ut elit consequat semper. Mauris ultrices gravida nibh, a sollicitudin lorem iaculis sed. Integer fringilla accumsan lorem, a rutrum orci blandit sit amet. Donec ultrices libero a ante accumsan, vel semper orci posuere. Curabitur eget urna imperdiet, vulputate neque dapibus, pellentesque nisl. Ut felis mi, ultrices non dui nec, rhoncus efficitur est.\n\nNulla cursus ac enim ut cursus. Vivamus venenatis eros vitae fringilla dapibus. Nunc commodo pharetra diam quis interdum. Sed imperdiet nisi vulputate mi malesuada eleifend. Vivamus iaculis sollicitudin dolor. Phasellus erat urna, elementum in feugiat vitae, feugiat viverra elit. Donec lacinia nulla ut lorem aliquet, ac ultrices quam ultricies. Proin euismod libero et efficitur rutrum. Donec vehicula, nisl in finibus maximus, libero justo consectetur leo, quis auctor quam ex in arcu. Class aptent taciti sociosqu ad litora torquent per conubia nostra, per inceptos himenaeos. Phasellus sit amet eros ultrices, vehicula erat vitae, viverra risus. Lorem ipsum dolor sit amet, consectetur adipiscing elit. Aenean vulputate consectetur malesuada.\n\nVestibulum ac justo viverra, pellentesque ipsum quis, sagittis nibh. Vivamus sed est velit. Cras faucibus leo non leo imperdiet dapibus. Aenean sit amet justo vel magna tempor dictum. Aliquam accumsan varius pellentesque. Etiam suscipit dui nec est consectetur auctor eget eget quam. Nullam tincidunt fringilla nulla, imperdiet porttitor dolor dignissim ut. Aenean rutrum sagittis viverra. Mauris nec tellus eu massa sodales pretium. Curabitur interdum sollicitudin erat at congue.\n\nAenean tristique elit neque. Vivamus feugiat tempus nisl, ut dictum lectus rhoncus at. Cras et tincidunt tellus, id feugiat tellus. Proin aliquam felis at sem tempor dictum. Aliquam rutrum ipsum sapien, in commodo diam lacinia vitae. Aliquam pellentesque sem ex, ac pulvinar nibh pulvinar non. Donec et odio eu arcu pretium euismod vitae ut erat. Donec ut nisi feugiat, convallis sapien id, convallis massa. Nam a odio ante. Integer scelerisque elit at magna iaculis pellentesque. Praesent eu lacinia nisl, a dictum dui. Sed id pellentesque lectus. Pellentesque sed efficitur lorem. Praesent rutrum ex et enim consectetur, a bibendum nisi auctor. Proin at metus porttitor, dictum ex ac, tincidunt odio. Sed quis justo leo.\n\nNunc scelerisque ex tortor, in elementum diam viverra sed. Nullam ut lorem posuere, dignissim sem at, fermentum augue. In aliquet, urna ac ullamcorper laoreet, diam tellus tincidunt sem, et euismod justo sem a est. Mauris suscipit varius vulputate. Etiam ullamcorper velit sit amet nisi varius, ac lobortis nulla lacinia. Ut ultrices pretium urna, ut luctus leo interdum quis. Sed molestie eleifend lorem vel luctus. In id mi non erat tristique fermentum vitae nec elit. Nullam sit amet diam at ante pulvinar lobortis. Vivamus suscipit feugiat ipsum, vel scelerisque dolor rhoncus in. Maecenas sed congue sapien. Quisque finibus ac magna sit amet hendrerit.\n\n";

    TextBox body(display.get(), 
        display.get_width() / 2.0f, display.get_height() / 2.0f + 25.0f,
        title.width, display.get_height() - 150.0f,
        txt, font_body, al_map_rgb(0,0,0));
    body.border.enabled = true;
    body.border.thickness = 2.0f;
    body.border.color = al_map_rgb(0,155,0);
    body.border.padding = 4.0f;
    body.fill.enabled = true;
    body.fill.color = al_map_rgb(0,155,0);
    body.align.vertical = Align::CENTER;
    body.align.horizontal = Align::CENTER;
    body.text.padding = 4.0f;
    body.text.flags = ALLEGRO_ALIGN_LEFT;
    body.text.multiline = true;
    body.editable = true;

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
            title.handle_event(event);
            body.handle_event(event);
            switch (event.type) {
            case ALLEGRO_EVENT_DISPLAY_CLOSE:
                if (event.display.source == display) {
                    play = false;
                } break;
            case ALLEGRO_EVENT_DISPLAY_RESIZE:
                if (event.display.source == display) {
                    title.x = event.display.width / 2.0f;
                    body.x = event.display.width / 2.0f;
                    body.y = event.display.height / 2.0f + 25.0f;
                    body.height = event.display.height - 150.0f;
                    camera.viewport.width = event.display.width;
                    camera.viewport.height = event.display.height;
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
        title.render();
        body.render();
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
