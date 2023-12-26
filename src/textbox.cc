#include "textbox.hh"
#include <allegro5/allegro_primitives.h>

/////////////////////////////////////////////////////////////
// SYMBOL SETS

namespace {

    const unsigned
        LOWERCASE = 0,
        UPPERCASE = 1;

}

/////////////////////////////////////////////////////////////
// EXTERNAL API

// void TextBox::handle_event(const ALLEGRO_EVENT &event) {
//     switch (event.type) {
//     case ALLEGRO_EVENT_KEY_DOWN:
//         keyboard.press(event.keyboard.keycode);
//         break;
//     case ALLEGRO_EVENT_KEY_UP:
//         keyboard.release(event.keyboard.keycode);
//         break;
//     }
// }

void TextBox::update(double dt, Keyboard::Poll input) {
    // std::cout<<"update\n";
    
    unsigned mode = LOWERCASE;
    
    // if (input[ALLEGRO_KEY_CAPSLOCK])
    //     capslock = !capslock;
    if (input[ALLEGRO_KEY_LSHIFT] || input[ALLEGRO_KEY_RSHIFT])
        mode = UPPERCASE;

    if (input[ALLEGRO_KEY_DOWN]) {
        scroll += 5.0f;
    } if (input[ALLEGRO_KEY_UP]) {
        scroll -= 5.0f;
    }

    for (int keycode = 0; keycode < ALLEGRO_KEY_MAX; keycode++) {
        if (input[keycode] == KEY_REMAINED_PRESSED) {
            cooldown[keycode] -= dt;
            if (cooldown[keycode] > 0.0) {
                // if still on cooldown, then ignore polled key status
                input[keycode] = 0;
            }
        }
    }

    if (input[ALLEGRO_KEY_DELETE]) {
        text.clear();
    } if (input[ALLEGRO_KEY_TAB]) {
        text.push_back(' ');
        text.push_back(' ');
        text.push_back(' ');
        text.push_back(' ');
    } if (input[ALLEGRO_KEY_BACKSPACE]) {
        if (!text.empty())
            text.pop_back();
    }

    for (int keycode = 0; keycode < ALLEGRO_KEY_MAX; keycode++) {
        if (input[keycode]) {
            if (ENGLISH[mode][keycode])
                text.push_back(ENGLISH[mode][keycode]);
            cooldown[keycode] = 0.175;
        }
    }

}

// al_draw_filled_rectangle(
//     x, y,
//     x+width, y+height,
//     al_map_rgb(0, 155, 0)
// );

void TextBox::render() const {

    al_set_clipping_rectangle(
        x + border.thickness + padding,
        y + border.thickness + padding,
        width - 2.0f*border.thickness - 2.0f*padding,
        height - 2.0f*border.thickness - 2.0f*padding
    );

    al_draw_multiline_text(
        font,
        color,
        x + border.thickness + padding,
        y + border.thickness + padding - scroll,
        width - 2.0f*border.thickness - 2.0f*padding,
        0, 0,
        text.c_str()
    );

    al_reset_clipping_rectangle();

    if (border.enabled) {
        al_draw_rectangle(
            x, y,
            x+width, y+height,
            border.color,
            border.thickness
        );
    }

}