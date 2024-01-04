#include "app.hh"
#include <iostream>

///////////////////////////////////////////////////////////////////////
// PUBLIC API
//////////

App::App(float width, float height, double fixed_time_step) 
    : fixed_time_step(fixed_time_step)
    , display(width, height)
    , event_queue()
    , textbox(10.0f, 40.0f, width-20.0f, height-50.0f)
    , fps_counter(10.0f, 10.0f, width-20.0f, 20.0f)
    , rectangle(18.0f, 18.0f, al_map_rgb(255,0,0), 0.0f, 4.0f, 4.0f)
    , x(width/2.0f), y(height/2.0f)
    , keyboard()
{
    std::cout << "App()" << '\n';
    
    event_queue.pause(true);
    event_queue.add_source(al_get_keyboard_event_source());
    event_queue.add_source(display.get_event_source());
    // event_queue.add_source(fixed_update_timer.get_event_source());

    textbox.padding = 10.0f;
    textbox.color = al_map_rgb(0, 155, 0);
    textbox.border.enabled = true;
    textbox.border.thickness = 5.0f;
    textbox.border.color = al_map_rgb(255,255,255);
    textbox.text = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Nam sit amet mauris quis tortor consequat maximus a sit amet est. Aliquam ullamcorper lorem eu nunc ultrices sollicitudin. Quisque est nunc, scelerisque pellentesque luctus at, ultricies in lorem. Nam malesuada commodo suscipit. Donec rhoncus, ipsum a condimentum gravida, mauris mauris facilisis magna, id interdum metus felis eget libero. Quisque condimentum risus ut elit consequat semper. Mauris ultrices gravida nibh, a sollicitudin lorem iaculis sed. Integer fringilla accumsan lorem, a rutrum orci blandit sit amet. Donec ultrices libero a ante accumsan, vel semper orci posuere. Curabitur eget urna imperdiet, vulputate neque dapibus, pellentesque nisl. Ut felis mi, ultrices non dui nec, rhoncus efficitur est.\n\n";
    textbox.text += "Nulla cursus ac enim ut cursus. Vivamus venenatis eros vitae fringilla dapibus. Nunc commodo pharetra diam quis interdum. Sed imperdiet nisi vulputate mi malesuada eleifend. Vivamus iaculis sollicitudin dolor. Phasellus erat urna, elementum in feugiat vitae, feugiat viverra elit. Donec lacinia nulla ut lorem aliquet, ac ultrices quam ultricies. Proin euismod libero et efficitur rutrum. Donec vehicula, nisl in finibus maximus, libero justo consectetur leo, quis auctor quam ex in arcu. Class aptent taciti sociosqu ad litora torquent per conubia nostra, per inceptos himenaeos. Phasellus sit amet eros ultrices, vehicula erat vitae, viverra risus. Lorem ipsum dolor sit amet, consectetur adipiscing elit. Aenean vulputate consectetur malesuada.\n\n";
    textbox.text += "Vestibulum ac justo viverra, pellentesque ipsum quis, sagittis nibh. Vivamus sed est velit. Cras faucibus leo non leo imperdiet dapibus. Aenean sit amet justo vel magna tempor dictum. Aliquam accumsan varius pellentesque. Etiam suscipit dui nec est consectetur auctor eget eget quam. Nullam tincidunt fringilla nulla, imperdiet porttitor dolor dignissim ut. Aenean rutrum sagittis viverra. Mauris nec tellus eu massa sodales pretium. Curabitur interdum sollicitudin erat at congue.\n\n";
    textbox.text += "Aenean tristique elit neque. Vivamus feugiat tempus nisl, ut dictum lectus rhoncus at. Cras et tincidunt tellus, id feugiat tellus. Proin aliquam felis at sem tempor dictum. Aliquam rutrum ipsum sapien, in commodo diam lacinia vitae. Aliquam pellentesque sem ex, ac pulvinar nibh pulvinar non. Donec et odio eu arcu pretium euismod vitae ut erat. Donec ut nisi feugiat, convallis sapien id, convallis massa. Nam a odio ante. Integer scelerisque elit at magna iaculis pellentesque. Praesent eu lacinia nisl, a dictum dui. Sed id pellentesque lectus. Pellentesque sed efficitur lorem. Praesent rutrum ex et enim consectetur, a bibendum nisi auctor. Proin at metus porttitor, dictum ex ac, tincidunt odio. Sed quis justo leo.\n\n";
    textbox.text += "Nunc scelerisque ex tortor, in elementum diam viverra sed. Nullam ut lorem posuere, dignissim sem at, fermentum augue. In aliquet, urna ac ullamcorper laoreet, diam tellus tincidunt sem, et euismod justo sem a est. Mauris suscipit varius vulputate. Etiam ullamcorper velit sit amet nisi varius, ac lobortis nulla lacinia. Ut ultrices pretium urna, ut luctus leo interdum quis. Sed molestie eleifend lorem vel luctus. In id mi non erat tristique fermentum vitae nec elit. Nullam sit amet diam at ante pulvinar lobortis. Vivamus suscipit feugiat ipsum, vel scelerisque dolor rhoncus in. Maecenas sed congue sapien. Quisque finibus ac magna sit amet hendrerit.\n\n";

    fps_counter.padding = 5.0f;

}

App::~App() {
    std::cout << "~App()" << '\n';
}

void App::run() {
    std::cout << "run()" << '\n';

    ALLEGRO_EVENT event;
    bool play = true, step = true;

    /////////////////////////////////////
    // LOAD GRAPHICS
    /////////////////////////////////////
    // UNPAUSE
    event_queue.pause(false);
    // fixed_update_timer.start();
    double lag = 0.0, previous_time = al_get_time();
    /////////////////////////////////////
    // MAIN LOOP
    while (play) {

        if (event_queue.next(event)) {
            play = handle_event(event);
        } else {
            // problem: branch is unreachable if event queue is never empty
            // problem: running long enough may cause time to overflow

            double current_time = al_get_time();
            play = update(current_time - previous_time);
            
            lag += current_time - previous_time;
            while (play && lag >= fixed_time_step) {
                lag -= fixed_time_step;
                play = fixed_update();
            }
            
            render();
            previous_time = current_time;  
        }

    }
    /////////////////////////////////////
    // PAUSE
    event_queue.pause(true);
    event_queue.flush();
    // fixed_update_timer.stop();
    /////////////////////////////////////

}

///////////////////////////////////////////////////////////////////////
// PRIVATE API
//////////

bool App::handle_event(const ALLEGRO_EVENT &event) {

    switch (event.type) {
    case ALLEGRO_EVENT_DISPLAY_CLOSE:
        return false;
        break;
    case ALLEGRO_EVENT_DISPLAY_RESIZE:
        display.acknowledge_resize();
        textbox.width = event.display.width - 20.0f;
        textbox.height = event.display.height - 50.0f;
        break;
    case ALLEGRO_EVENT_KEY_DOWN:
        if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
            return false;
        keyboard.press(event.keyboard.keycode);
        break;
    case ALLEGRO_EVENT_KEY_UP:
        keyboard.release(event.keyboard.keycode);
        break;
    }

    return true;

}

// visual effects, things that don't change game logic
// things that don't need to be syncronized in online multiplayer
bool App::update(double delta_time) {

    // ui goes here
    fps_counter.text.clear();
    auto input = keyboard.poll();
    if (input[ALLEGRO_KEY_UP]) {
        fps_counter.text += "UP ";
    } if (input[ALLEGRO_KEY_DOWN]) {
        fps_counter.text += "DOWN ";
    } if (input[ALLEGRO_KEY_RIGHT]) {
        fps_counter.text += "RIGHT ";
    } if (input[ALLEGRO_KEY_LEFT]) {
        fps_counter.text += "LEFT ";
    }
    
    // fps_counter.text += "X=";
    // fps_counter.text += std::to_string(x);
    // fps_counter.text += " Y=";
    // fps_counter.text += std::to_string(y);

    // fps_counter.text += "#";
    
    // textbox.update(delta_time, input);

    return true;

}

// physics, things that change game logic
// things that need to be syncronized in online multiplayer
bool App::fixed_update() {

    // fps_counter.text = std::to_string(int(frame_rate));
    // frame_rate=0;
    
    auto input = keyboard.poll();
    if (input[ALLEGRO_KEY_UP]) {
        y -= 5.0f;
    } if (input[ALLEGRO_KEY_DOWN]) {
        y += 5.0f;
    } if (input[ALLEGRO_KEY_RIGHT]) {
        x += 5.0f;
    } if (input[ALLEGRO_KEY_LEFT]) {
        x -= 5.0f;
    }

    return true;

}

void App::render() {

    al_clear_to_color(al_map_rgb(0, 0, 0));
    // textbox.render();
    fps_counter.render();
    rectangle.render(x,y);
    al_flip_display();

}

/**
 * UI {
 *      handle_event()
 * Entity {
 *      update()
 *      fixed_update()
 *      render()
**/