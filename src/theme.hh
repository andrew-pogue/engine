#pragma once
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>

struct Theme {
    const ALLEGRO_FONT *font; // non-owning
    ALLEGRO_COLOR color;
};

