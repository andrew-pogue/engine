#include <allegro5/allegro5.h>
#include <allegro5/allegro_primitives.h>
#include "textfield.hh"
#include "text.hh"

///////////////////////////////////////////////////////////////////////////////
/// PUBLIC API

TextField::TextField(Rectangle area, String &&string)
    : Widget(area)
    , string(string), cursor_index{0}
    , scroll{0.f}, spacing{0.f}, padding{0.f, 0.f}
    , align{AlignX::LEFT, AlignY::TOP}
{ }

void TextField::render(Theme theme) const {
    const bool was_drawing_held = al_is_bitmap_drawing_held();
    al_hold_bitmap_drawing(true);
    int prev_clip_x=0, prev_clip_y=0, prev_clip_w=0, prev_clip_h=0;
    al_get_clipping_rectangle(&prev_clip_x, &prev_clip_y, &prev_clip_w, &prev_clip_h);

    auto bounds = area();
    bounds.x += padding.x; bounds.width -= padding.x * 2.f;
    bounds.y += padding.y; bounds.height -= padding.y * 2.f;
    al_set_clipping_rectangle(bounds.x, bounds.y, bounds.width, bounds.height);
    draw_text_with_effects(
        bounds, align, spacing, scroll, theme.font, theme.color, string.get(),
        CursorTextEffect(theme.font, theme.color, cursor_index));

    al_set_clipping_rectangle(prev_clip_x, prev_clip_y, prev_clip_w, prev_clip_h);
    al_hold_bitmap_drawing(was_drawing_held);
}

bool TextField::handle_event(const ALLEGRO_EVENT &event) {
    bool is_handled = false;
    switch(event.type) {
    case ALLEGRO_EVENT_KEY_CHAR:
        handle_input(event.keyboard.keycode, event.keyboard.unichar);
        is_handled = true; break;
    case ALLEGRO_EVENT_MOUSE_AXES:
        scroll += float(20 * event.mouse.dz);
        is_handled = true; break;
    }
    return is_handled;
}

CursorTextEffect::CursorTextEffect(const ALLEGRO_FONT *font, ALLEGRO_COLOR color, int index)
    : font(font), color(color), index(index)
{ }

void CursorTextEffect::operator()(int index, int ln, int &ch, float &x, float &y, ALLEGRO_COLOR &color) {
    if (index != this->index) return;
    float h = al_get_font_line_height(font);
    al_draw_filled_rectangle(x - 4.f, y - 2.f, x, y + h + 2.f, this->color);
}

///////////////////////////////////////////////////////////////////////////////
/// PRIVATE API

void TextField::handle_input(int keycode, int unichar) {
    switch(keycode) {
    case ALLEGRO_KEY_LEFT:          
        if (cursor_index > 0) {
            cursor_index--;
        } break;
    case ALLEGRO_KEY_RIGHT:
        if (cursor_index < string.length()) {
            cursor_index++;
        } break;
    case ALLEGRO_KEY_BACKSPACE:
        if (cursor_index > 0) {
            string.erase(--cursor_index);
        } break;
    case ALLEGRO_KEY_DELETE:
        if (cursor_index >= 0 && cursor_index < string.length()) {
            string.erase(cursor_index);
        } break;
    case ALLEGRO_KEY_ENTER: string.append('\n'); break;
    //case ALLEGRO_KEY_TAB: string.append('\t'); break;
    case ALLEGRO_KEY_ESCAPE: break;
    default:
        if (unichar > 0) {
            string.insert(cursor_index++, unichar);
        } break;
    }
}

