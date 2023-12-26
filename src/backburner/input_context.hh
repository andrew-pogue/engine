#pragma once

#include <allegro5/allegro5.h>
#include <forward_list>
#include <functional>
#include <map>

struct Command {
    std::function<void(void)> execute;
    unsigned cooldown=0;
    unsigned delay=0;
    operator bool() const { return (bool)execute; }
};

class InputContext {
public:

    void handle_input(const std::forward_list<int> &input) {
        for (int i : input) {
            auto it = command_.find(i);
            if (it != command_.end()) {
                if (it->second.delay > 0) {
                    it->second.delay--;
                } else if (it->second.execute) {
                    it->second.execute();
                    it->second.delay = it->second.cooldown;
                }
            }
        }
    }

    // Gets the command mapped to the given button ID.
    // If no command is found, creates and maps an empty command to the ID.
    Command &on_input(int keycode) {
        return command_[keycode];
    }

    // // Maps the given command to the given button ID.
    // bool set_command(int keycode, Command &&command) {
    //     return command_.insert({keycode, command}).second;
    // }

    // // Gets the command mapped to the given button ID.
    // // If no command is found, throws an exception.
    // Command &get_command(int keycode) {
    //     return command_.at(keycode);
    // }

private:

    std::map<int, Command> command_;

};

// context[ALLEGRO_KEY_A] = go_left();
// context[ALLEGRO_KEY_SHIFT] = change_context();

// context.command(ALLEGRO_KEY_A) = [&message](){ message += 'a'; };
// context.command(ALLEGRO_KEY_SHIFT, ALLEGRO_KEY_A) = [&message](){ message += 'A'; };

// context_modifiers[ALLEGRO_KEY_SHIFT] = [&capitalize](){capitalize=true;}



// layers of contexts, input is passed to the top-most context layer, once that layer closes,
// input is passed to the next layer, if all layers are closed, input is not processed

// if keys are pressed at the same time assume not coincidence
// check if keys linked together go to commmand
// if not then check keys individuallly to their own commands


// context queue?
// contexts are pushed onto queue, input is passed to top queue
// input processed by context may or may not be passed to next context
// priority queue?

// condense InputContext and InputLog into InputHandler
// pass event into InputHandler
// allow InputHandler to take input from multiple devices (keyboard and mouse)
