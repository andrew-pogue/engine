#pragma once
#include <allegro5/allegro5.h>
#include <allegro5/allegro_primitives.h>
#include "font.hh"
#include "keyboard.hh"
// #include <iostream>

struct Text {
    ALLEGRO_USTR *string;
    Font font;
    ALLEGRO_COLOR color;
    int flags;
    ~Text() { al_ustr_free(string); }
};

struct Fill {
    bool enabled;
    ALLEGRO_COLOR color;
};

struct Border {
    bool enabled;
    float thickness, offset;
    ALLEGRO_COLOR color;
};

struct Align {
    enum : int {
        TOP = 0b00,
        BOTTOM = 0b01,
        CENTER = 0b10,
        INTEGER = 0b11,
        LEFT = TOP,
        RIGHT = BOTTOM,
    };

    int horizontal : 3 = 0;
    int vertical : 3 = 0;
};

class TextBox {
private:

    struct DrawLineParams {
        Text &text;
        float x, y, w, h, scroll;
    };

    static bool draw_line(int line_num, const ALLEGRO_USTR *line, void *extra) {
        DrawLineParams params = *(DrawLineParams *)extra;
        const float h = params.text.font.get_line_height(),
              y = params.y + line_num * h - params.scroll;
        if (y + h < params.y) return true;
        if (y > params.y + params.h) return false;
        ALLEGRO_USTR_INFO info;
        al_draw_ustr(
            params.text.font.get(),
            params.text.color,
            params.x, y,
            params.text.flags,
            al_ref_ustr(&info, line, 0, al_ustr_size(line)));
        return true;
    }

    void draw_multiline_text(float i, float j, float w, float h) {
        // printf("x:%f y:%f w:%f h:%f\n", i, j, w, h);
        // al_set_clipping_rectangle(i, j, w, h);
        if (align.horizontal = Align::CENTER) i += w / 2.0f;
        DrawLineParams params{text, i, j, w, h, scroll};
        al_do_multiline_ustr(text.font.get(), w, text.string, draw_line, &params);
        // al_reset_clipping_rectangle();
        al_draw_rounded_rectangle(i, j, i+w, j+h, rx, ry, al_map_rgb(255,255,255), 2.0f); 
    }

public:
    
    ALLEGRO_DISPLAY *display;
    float x, y, width, height, rx, ry, padding, scroll;

    Text text;
    Fill fill;
    Border border;
    Align align;

    TextBox(ALLEGRO_DISPLAY *display, float x, float y, float w, float h, const char *str, const Font &font, ALLEGRO_COLOR color)
        : display(display)
        , x(x), y(y), width(w), height(h)
        , rx(0.0f), ry(0.0f), padding(0.0f), scroll(0.0f)
        , text{al_ustr_new(str), font, color, ALLEGRO_ALIGN_LEFT}
        , fill{false, al_map_rgb(255, 255, 255)}
        , border{true, 2.0f, 0.0f, al_map_rgb(255, 255, 255)}
        , align{Align::RIGHT, Align::BOTTOM}
    { }

    void handle_event(const ALLEGRO_EVENT &event) {
        switch(event.type) {
        case ALLEGRO_EVENT_KEY_CHAR:
            if (event.keyboard.display == display && event.keyboard.unichar > 0) {
                switch(event.keyboard.keycode) {
                case ALLEGRO_KEY_BACKSPACE:
                    al_ustr_remove_chr(text.string, al_ustr_offset(text.string, -1));
                    break;
                case ALLEGRO_KEY_DELETE:
                    break;
                case ALLEGRO_KEY_ENTER:
                    break;
                case ALLEGRO_KEY_TAB:
                    break;
                case ALLEGRO_KEY_ESCAPE:
                    break;
                default:
                    al_ustr_append_chr(text.string, event.keyboard.unichar);
                    break;
                }
            } break;
        case ALLEGRO_EVENT_MOUSE_AXES:
            if (event.mouse.display == display) {
                scroll += float(5 * event.mouse.dz);
            } break;
        }
    }

    void render() {
        const float
            x1 = x - float(align.horizontal == Align::LEFT) * width
                - float(align.horizontal == Align::CENTER) * width / 2.0f,
            y1 = y - float(align.vertical == Align::TOP) * height
                - float(align.vertical == Align::CENTER) * height / 2.0f,
            x2 = x1 + width,
            y2 = y1 + height;
        if (fill.enabled) al_draw_filled_rounded_rectangle(x1, y1, x2, y2, rx, ry, fill.color);
        if (border.enabled) al_draw_rounded_rectangle(
            x1 + border.offset, y1 - border.offset,
            x2 + border.offset, y2 - border.offset,
            rx, ry, border.color, border.thickness);
        draw_multiline_text(
            x1 + padding, y1 + padding,
            x2 - x1 - padding, y2 - y1 - padding);
        al_draw_filled_rectangle(x - 5.0f, y - 5.0f, x + 5.0f, y + 5.0f, al_map_rgb(255,255,255));
    }

};

