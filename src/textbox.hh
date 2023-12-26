#pragma once

#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <array>
#include <string>
#include <iostream>

#include "keyboard.hh"

// TODO: capslock? numlock?

class TextBox {
public:

    TextBox(float x, float y, float width, float height)
        : text()
        , width(width), height(height)
        , x(x), y(y)
        , padding(0.0f)
        , border{false, al_map_rgb(255, 255, 255), 0.0f}
        , color(al_map_rgb(255,255,255))
        , cooldown()
        , font(al_create_builtin_font())
        , scroll(0.0f)
    {
    
        if (!font) throw "Failed to create font.";
        cooldown.fill(0.0);
    
    }

    ~TextBox() {
    
        al_destroy_font(font);
    
    }

    void handle_event(const ALLEGRO_EVENT &);

    void update(double, Keyboard::Poll);

    void render() const;

    std::string text;

    float width, height,
        x, y,
        padding;

    struct {
        bool enabled;
        ALLEGRO_COLOR color;
        float thickness;
    } border;

    ALLEGRO_COLOR color;

private:

    // const std::array<char, ALLEGRO_KEY_MAX> symbols;
    std::array<double, ALLEGRO_KEY_MAX> cooldown;
    ALLEGRO_FONT *font;
    float scroll;

    // bool capslock;

};