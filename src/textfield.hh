#pragma once
#include <allegro5/allegro5.h>
#include <vector>
#include "layout.hh"
#include "string.hh"
#include "text.hh"
#include "vector2.hh"
#include "widget.hh"

struct TextField : Widget {

    String string;
    int cursor_index;
    float scroll, spacing;
    Vector2<float> padding;
    Align align;

    TextField(Rectangle area, String &&string);
    void render(Theme theme) const override;
    bool handle_event(const ALLEGRO_EVENT &event) override;
    
    // TODO: on_enter on_tab on_escape callbacks
    // TODO: smoother scrolling
    // TODO: cursor move up and down
    // TODO: insertion mode
    // TODO: selection

private:

    void handle_input(int keycode, int unichar);

};

struct CursorTextEffect : TextEffect {

    const ALLEGRO_FONT *font;
    const ALLEGRO_COLOR color;
    const int index;

    CursorTextEffect(const ALLEGRO_FONT *font, ALLEGRO_COLOR color, int index);
    void operator()(int index, int ln, int &ch, float &x, float &y, ALLEGRO_COLOR &color) override;

};

