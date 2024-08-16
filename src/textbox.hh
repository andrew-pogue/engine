#pragma once
#include <allegro5/allegro5.h>
#include <allegro5/allegro_primitives.h>
#include "font.hh"
#include "keyboard.hh"

struct Text {
    ALLEGRO_USTR *string;
    Font font;
    ALLEGRO_COLOR color;
    int flags = 0;
    float padding = 0.0f, offset = 0.0f;
    bool multiline = false;
    ~Text() { al_ustr_free(string); }
};

struct Fill {
    bool enabled;
    ALLEGRO_COLOR color;
};

struct Border {
    bool enabled;
    float thickness, offset, padding;
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
        float x, y, w, h;
    };

    static bool draw_line(int line_num, const ALLEGRO_USTR *line, void *extra) {
        DrawLineParams params = *(DrawLineParams *)extra;
        const float h = params.text.font.get_line_height(),
              y = params.y + line_num * h - params.text.offset;
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
        al_set_clipping_rectangle(i, j, w, h);
        if (text.flags == ALLEGRO_ALIGN_CENTRE) i += w / 2.0f;
        DrawLineParams params{text, i, j, w, h};
        al_do_multiline_ustr(text.font.get(), w, text.string, draw_line, &params);
        al_reset_clipping_rectangle();
    }

    void draw_text(float i, float j, float w, float h) {
        al_set_clipping_rectangle(i, j, w, h);
        if (text.flags == ALLEGRO_ALIGN_CENTRE) {
            i += w / 2.0f;
            j += h / 2.0f - text.font.get_line_height() / 2.0f;
        }
        al_draw_ustr(text.font.get(), text.color, i, j, text.flags, text.string); 
        al_reset_clipping_rectangle();
    }

public:
    
    ALLEGRO_DISPLAY *display;
    float x, y, width, height, rx, ry;
    bool editable;

    Text text;
    Fill fill;
    Border border;
    Align align;

    TextBox(ALLEGRO_DISPLAY *display, float x, float y, float w, float h, const char *str, const Font &font, ALLEGRO_COLOR color)
        : display(display)
        , x(x), y(y), width(w), height(h)
        , rx(0.0f), ry(0.0f)
        , editable(false)
        , text{al_ustr_new(str), font, color, ALLEGRO_ALIGN_LEFT, 0.0f, 0.0f, false}
        , fill{false, al_map_rgb(255, 255, 255)}
        , border{true, 2.0f, 0.0f, 0.0f, al_map_rgb(255, 255, 255)}
        , align{Align::RIGHT, Align::BOTTOM}
    { }

    void handle_event(const ALLEGRO_EVENT &event) {
        switch(event.type) {
        case ALLEGRO_EVENT_KEY_CHAR:
            if (event.keyboard.display == display && event.keyboard.unichar > 0 && editable) {
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
                text.offset += float(20 * event.mouse.dz);
            } break;
        }
    }

    void render() {
        float
            x1 = x - float(align.horizontal == Align::LEFT) * width
                - float(align.horizontal == Align::CENTER) * width / 2.0f,
            y1 = y - float(align.vertical == Align::TOP) * height
                - float(align.vertical == Align::CENTER) * height / 2.0f,
            x2 = x1 + width,
            y2 = y1 + height;
        if (fill.enabled) al_draw_filled_rounded_rectangle(x1, y1, x2, y2, rx, ry, fill.color);
        if (border.enabled) al_draw_rounded_rectangle(
            x1 - border.padding - border.thickness + border.offset, y1 - border.padding - border.thickness + border.offset,
            x2 + border.padding + border.thickness + border.offset, y2 + border.padding + border.thickness + border.offset,
            rx, ry, border.color, border.thickness);
        x1 += text.padding; y1 += text.padding;
        x2 -= text.padding; y2 -= text.padding;
        if (text.multiline) draw_multiline_text(x1, y1, x2 - x1, y2 - y1);
        else draw_text(x1, y1, x2 - x1, y2 - y1);
        al_draw_filled_rectangle(x - 5.0f, y - 5.0f, x + 5.0f, y + 5.0f, al_map_rgb(255,255,255));
    }

};

