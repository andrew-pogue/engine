#pragma once
#include <allegro5/allegro5.h>
#include <allegro5/allegro_primitives.h>
#include <functional>
#include "font.hh"
#include "keyboard.hh"

class TextBox : Widget {
public:

    Text text;
    Align align;

    struct Fill {
        Color color;
    } fill;

    struct Border {
        float thickness;
        Vector2f padding;
        Color color;
    } border;

    Vector2f rounding;

    struct Flags {
        bool border : 1;
        bool fill : 1;
        bool editable : 1;
        bool multiline : 1;
    } flags;

    TextBox(String string, Font font, Color color)
        : text{String(str), font, color}
        , align{Align::NONE, Align::NONE}
        , fill{color.get_complement()}
        , border{0.0f, {0.0f, 0.0f}, color}
        , rounding{0.0f, 0.0f}
        , flags{false, false, false, false}
    { }

    void render(float x, float y) const override {
        float
            x1 = x - float(align.horizontal == Align::LEFT) * w
                - float(align.horizontal == Align::CENTER) * w / 2.0f,
            y1 = y - float(align.vertical == Align::TOP) * h
                - float(align.vertical == Align::CENTER) * h / 2.0f,
            x2 = x1 + w,
            y2 = y1 + h;
        if (flags.fill) al_draw_filled_rounded_rectangle(x1, y1, x2, y2, rx, ry, fill.color);
        if (flags.border) al_draw_rounded_rectangle(
            x1 - border.padding - border.thickness, y1 - border.padding - border.thickness,
            x2 + border.padding + border.thickness, y2 + border.padding + border.thickness,
            rx, ry, border.color, border.thickness);
        x1 += text.padding; y1 += text.padding;
        x2 -= text.padding; y2 -= text.padding;
        ScopedClippingRectangle clip(x1, y1, x2 - x1, y2 - y1);
        if (flags.multiline) text.draw_multiline(x1, y1, x2 - x1, y2 - y1);
        else text.draw(x1, y1);
    }

    bool handle_event(const ALLEGRO_EVENT &event) override {
        bool is_handled = false;
        switch(event.type) {
        case ALLEGRO_EVENT_KEY_CHAR:    handle_input(event.keyboard.unichar); is_handled = true; break;
        case ALLEGRO_EVENT_MOUSE_AXES:  text.offset += float(20 * event.mouse.dz); is_handled = true; break;
        }
        return is_handled;
    }

private:

    void handle_input(int unichar) {
        if (!editable || unichar < 1) return;
        switch(event.keyboard.keycode) {
        case ALLEGRO_KEY_BACKSPACE:     string.erase(-1); break;
        case ALLEGRO_KEY_DELETE:        break;
        case ALLEGRO_KEY_ENTER:         flags.multiline ? string.append(unichar); break;
        case ALLEGRO_KEY_TAB:           string.append(unichar); break;
        case ALLEGRO_KEY_ESCAPE:        break;
        default:                        string.append(unichar); break;
        }
    }

};

