#pragma once
#include <vector>

void foo(std::vector<bool> poll) {
    int input = 0;

    struct {
        int move_up=0,
            move_down=1,
            move_right=2,
            move_left=3;
    } controls;

    // if (poll[controls.move_up]) move_up();
    // if (poll[controls.move_down]) move_down();
    // if (poll[controls.move_right]) move_right();
    // if (poll[controls.move_left]) move_left();




}


// input_handler.update(input_log.poll())

// commands[button]()
//



// instead of tieing keys to functionality
//  tie functionality to the key

// command[key] = move_left()

// function_key = key













// class {
// public:


//     //  controls.function_name = key
//     //  if key then function()
//     //      or
//     //  commands[key] = function
//     //  if key and commands[key] then commands[key]()

//     //  function fixed, key dynamic (if statemetns)
//     //  function order decided by implementor (easy)
//     //      or
//     //  key fixed, function dynamic (no if statemetns)
//     //  function order decided by assignment (complicated)

//     // key combos:  [shift] + [alt] + [d]
//     // context:     [in-game menus] vs [text boxes] vs [player movement]

//     // who decides what commands are processed before others
//     // who decides what to do with input
//     //      the input handler or the app itself?
//     //      the app itslef.

//     void handle_input(std::vector<int> poll) const {
//         for (auto key : poll) {
//             input_ += symbols_[key];
//         }
//     }

// private:

//     std::array<char, ALLEGRO_KEY_MAX> symbols_;
//     std::string input_;

// } keyboard;

// INTERNAL API

// void App::handle_input() {

//     for (auto input : keyboard_.poll()) {
//         switch (input) {
//         case ALLEGRO_KEY_ENTER:
//             message_.clear();
//             break;
//         case ALLEGRO_KEY_BACKSPACE:
//             if (message_.size())
//                 message_.pop_back();
//             break;
//         case ALLEGRO_KEY_LSHIFT:
//         case ALLEGRO_KEY_RSHIFT:
//             break;
//         // case ALLEGRO_KEY_ESCAPE:
//         //     return false;
//         default:
//             message_ += keyvalue_.at(input);
//             break;
//         }
//     }

// }