#pragma once
#include <allegro5/allegro5.h>
#include "inputlog.hh"

using Keyboard = InputLog<ALLEGRO_KEY_MAX>;

using Symbols = std::array<char, ALLEGRO_KEY_MAX>;

const std::array<Symbols, 2> ENGLISH {
    // LOWERCASE SYMBOLS
    Symbols{ 0,
    // ALLEGRO_KEY_A ... ALLEGRO_KEY_Z
    'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j',
    'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't',
    'u', 'v', 'w', 'x', 'y', 'z',
    // ALLEGRO_KEY_0 ... ALLEGRO_KEY_9
    '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
    // ALLEGRO_KEY_PAD_0 ... ALLEGRO_KEY_PAD_9
    '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
    // ALLEGRO_KEY_F1 ... ALLEGRO_KEY_F12
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    // ALLEGRO_KEY_ESC ... ALLEGRO_KEY_COMMA
    0, '`', '-', '=', 0, 0, '[', ']', '\n', ';', '\'',
    // ALLEGRO_KEY_BACKSLASH ... ALLEGRO_KEY_LEFT
    '\\', '\\', ',', '.', '/', ' ', 0, 0, 0, 0, 0, 0, 0,
    // ALLEGRO_KEY_RIGHT ... ALLEGRO_KEY_KANA
    0, 0, 0, '/', '*', '-', '+', 0, '\n', 0, 0, 0, 0, 0,
    // ALLEGRO_KEY_CONVERT ... ALLEGRO_KEY_KANA
    0, 0, 0, 0, ';', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    // ALLEGRO_KEY_CONVERT ... ALLEGRO_KEY_NUMLOCK
    0, 0, 0, 0, ';', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    // ALLEGRO_KEY_CAPSLOCK ... ALLEGRO_KEY_COMMAND
    0, '=', '`', ';', 0 },
        
    // UPPERCASE SYMBOLS
    Symbols{ 0,
    // ALLEGRO_KEY_A ... ALLEGRO_KEY_Z
    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J',
    'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T',
    'U', 'V', 'W', 'X', 'Y', 'Z',
    // ALLEGRO_KEY_0 ... ALLEGRO_KEY_9
    ')', '!', '@', '#', '$', '%', '^', '&', '*', '(',
    // ALLEGRO_KEY_PAD_0 ... ALLEGRO_KEY_PAD_9
    '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
    // ALLEGRO_KEY_F1 ... ALLEGRO_KEY_F12
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    // ALLEGRO_KEY_ESC ... ALLEGRO_KEY_COMMA
    0, '~', '_', '+', 0, 0, '{', '}', '\n', ':', '\"',
    // ALLEGRO_KEY_BACKSLASH ... ALLEGRO_KEY_LEFT
    '|', '|', '<', '>', '?', ' ', 0, 0, 0, 0, 0, 0, 0,
    // ALLEGRO_KEY_RIGHT ... ALLEGRO_KEY_KANA
    0, 0, 0, '/', '*', '-', '+', 0, '\n', 0, 0, 0, 0, 0,
    // ALLEGRO_KEY_CONVERT ... ALLEGRO_KEY_KANA
    0, 0, 0, 0, ':', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    // ALLEGRO_KEY_CONVERT ... ALLEGRO_KEY_NUMLOCK
    0, 0, 0, 0, ':', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    // ALLEGRO_KEY_CAPSLOCK ... ALLEGRO_KEY_COMMAND
    0, '+', '~', ':', 0 }
};