#pragma once
#include "color.hh"
#include "font.hh"
#include "string.hh"

class Text : Widget {
public:

    String string;
    Font font;
    Color color;
    Align align;
    // float line_spacing;
    int scroll_speed;

    // std::function<bool(float x, float y)> per_line_affect;
    // std::function<bool(float x, float y)> per_char_affect;

    struct Flags {
        bool multline : 1 = false;
        bool editable : 1 = false;
        bool scrollable : 1 = false;
    } flags;

    Text(String string, Font font, Color color, Align align)
        : string(string), font(font), color(color), align(align)
        , scroll_speed(20)
    {}

    float get_string_height() {
        float height = 0.0f;
        al_do_multiline_ustr(font.get(), width, string.get(), get_height_helper, &height);
        return height;
    }
    
    void render(float x, float y) const {
        adjust_draw_coord(x, y);
        flags.multiline ? draw_multiline(x, y) : draw_line(x, y);
    }

    bool handle_event(const ALLEGRO_EVENT &event) override {
        bool is_handled = false;
        switch(event.type) {
        case ALLEGRO_EVENT_KEY_CHAR:
            handle_input(event.keyboard.unichar);
            is_handled = true; break;
        case ALLEGRO_EVENT_MOUSE_AXES:
            if (flags.scrollable) text.offset += float(scroll_speed * event.mouse.dz);
            is_handled = true; break;
        }
        return is_handled;
    }

private:

    struct Line {
        float x, y, max_y;
        int flags;
    };

    void draw_line(float x, float y) const {
        // per_line_affect ? per_line_affect(x, y);
        al_draw_ustr(font.get(), color, x, y, get_draw_flags(), string.get());
    }

    void draw_multiline(float x, float y) {
        x += (align.horizontal == Align::CENTER) * width / 2.0f;
        Line line{x, y, height, get_draw_flags()};
        al_do_multiline_ustr(font.get(), width, string.get(), draw_multiline_helper, &line);
    }

    bool get_height_helper(int line_num, const ALLEGRO_USTR *line, void *extra) {
        *(float *)extra += font.get_line_height();
        return true;
    }

    bool draw_multiline_helper(int line_num, const ALLEGRO_USTR *line, void *extra) {
        // is this slower than regular al_draw_multiline_ustr ?
        const Line &line = *(Line *)extra;
        const float h = font.get_line_height();
        float x = line.x,
              y = line.max_y + line_num * h;
        // per_line_affect ? per_line_affect(x, y);
        if (flags.scrollable && y + h < line.max_y) return true;
        if (y + h > line.max_y + line.h) return false;
        ALLEGRO_USTR_INFO info;
        al_draw_ustr(
            font.get(), color,
            line.x, y,
            line.flags,
            al_ref_ustr(&info, line, 0, al_ustr_size(line)));
        return true;
    }

    void adjust_draw_coord(float &x, float &y) {
        y += (align.vertical == Align::CENTER) * font.get_line_height() / 2.0f;
    }

    int get_draw_flags() {
        return (align.horizontal == Align::CENTER) ? ALLEGRO_ALIGN_CENTER : 0;
    }

    void handle_input(int unichar) {
        if (!flags.editable || unichar < 1) return;
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

