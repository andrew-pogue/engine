#include <allegro5/allegro5.h>
#include <allegro5/allegro_primitives.h>
#include <cctype>
#include <iostream>
#include "text-editor.hh"

// NOTE:
// cursor is drawn at the front of the character at its index
// the portion of the text after an insertion or deletion needs to be parsed
//      or the line info will be wrong

///////////////////////////////////////////////////////////////////////////////
/// PUBLIC API

TextEditor::TextEditor(Rectangle area, const ALLEGRO_FONT *font, ALLEGRO_COLOR color, String &&string)
    : Widget(area)
    , font(font), color(color)
    , scroll{0.f}, spacing{0.f}, padding{0.f, 0.f}
    , align{Align::LEFT, Align::TOP}
    , cursor{0}, lines(), chars()
{
    assert(font);
    chars.reserve(string.length());
    for (int ch : string) chars.push_back(ch);
    parse();
}

void TextEditor::render() const {
    if (!font) return;
    int prev_clip_x=0, prev_clip_y=0, prev_clip_w=0, prev_clip_h=0;
    al_get_clipping_rectangle(&prev_clip_x, &prev_clip_y, &prev_clip_w, &prev_clip_h);
    const bool was_drawing_held = al_is_bitmap_drawing_held();

    const float
        min_x = bounds.x + padding.x,
        min_y = bounds.y + padding.y,
        max_x = bounds.x + bounds.width - padding.x * 2.f,
        max_y = bounds.y + bounds.height - padding.y * 2.f;
    //al_draw_filled_rectangle(min_x, min_y, max_x, max_y, al_map_rgb(0,0,255));
    al_set_clipping_rectangle(min_x-2.f, min_y-2.f, max_x-min_x+4.f, max_y-min_y+4.f);

    const int line_height = al_get_font_line_height(font);
    int ln = 0;
    float y = min_y - scroll;

    while (ln < line_count() && y < min_y - line_height) {
        y += line_height + spacing;
        ln++;
    }

    al_hold_bitmap_drawing(true);
    while (ln < line_count() && y < max_y) {
        float x = min_x;
        if (align.x == Align::RIGHT)
            x += bounds.width - line_width(ln);
        if (align.x == Align::CENTER_X)
            x += (bounds.width - line_width(ln)) / 2.f;
        //al_draw_filled_rectangle(x, y, x + line_width(ln), y + line_height, al_map_rgb(0,0,255));
        int prev_ch = ALLEGRO_NO_KERNING;
        for (int i = line_begin(ln); i <= line_end(ln); i++) {
            int ch = chars.at(i);
            x += al_get_glyph_advance(font, prev_ch, ch);
            al_draw_glyph(font, color, x, y, ch);
            prev_ch = ch;
            if (i == cursor) {
                al_draw_filled_rectangle(
                    x - 2.f, y - 2.f,
                    x + 2.f, y + line_height + 2.f,
                    al_map_rgb(255,255,255)
                );
            }
        }
        y += line_height + spacing;
        ln++;
    }

    al_hold_bitmap_drawing(was_drawing_held);
    al_set_clipping_rectangle(prev_clip_x, prev_clip_y, prev_clip_w, prev_clip_h);
}

bool TextEditor::handle_event(const ALLEGRO_EVENT &event) {
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

int TextEditor::line_begin(int ln) const {
    if (ln < 0) ln += lines.size();
    int begin = lines.at(ln).begin;
    assert(begin >= 0 && begin < chars.size());
    return begin;
}

int TextEditor::line_end(int ln) const {
    if (ln < 0) ln += lines.size();
    int end = lines.at(ln).end;
    assert(end >= 0 && end < chars.size());
    return end;
}

int TextEditor::line_number(int i) const {
    if (i < 0) i += chars.size();
    assert(i >= 0);
    int ln = 0;
    while (ln < line_count() && i > line_end(ln)) ln++;
    return ln;
}

int TextEditor::line_width(int ln) const {
    if (ln < 0) ln += lines.size();
    return lines.at(ln).width;
}

void TextEditor::insert(int i, int ch) {
    if (i < 0) i += chars.size();
    chars.insert(chars.begin() + i, ch);
    parse(line_number(i));
}

void TextEditor::remove(int i) {
    if (i < 0) i += chars.size();
    chars.erase(chars.begin() + i);
    parse(line_number(i));
}

/* used for finding char index at mouse click
int find_index(float y, float x) {
    j = bounds.y
    while j < y && next line
        j += line heigh + spacing
    index = 0
    while i < x && next ch
        i += ch advance
        index++
    return --index;
}
*/

String TextEditor::to_string() const { 
    String string;
    std::vector<int> chars;
    for (int ch : chars) string.append(ch);
    return string;
}

///////////////////////////////////////////////////////////////////////////////
/// PRIVATE API

void TextEditor::handle_input(int keycode, int unichar) {
    switch(keycode) {
    case ALLEGRO_KEY_UP: {
            int cursor_ln = line_number(cursor);
            if (cursor_ln > 0) go_to_line(cursor_ln-1);
        } break;
    case ALLEGRO_KEY_DOWN: {
            int cursor_ln = line_number(cursor);
            if (cursor_ln < line_count()-1) go_to_line(cursor_ln+1);
        } break;
    case ALLEGRO_KEY_LEFT:
        if (cursor > 0) {
            cursor--;
        } break;
    case ALLEGRO_KEY_RIGHT:
        if (cursor < char_count()-1) {
            cursor++;
        } break;
    case ALLEGRO_KEY_BACKSPACE:
        if (cursor > 0) {
            remove(--cursor);
        } break;
    case ALLEGRO_KEY_DELETE:
        if (cursor >= 0 && cursor < char_count()-1) {
            remove(cursor);
        } break;
    case ALLEGRO_KEY_ENTER: {
            int cursor_ln = line_number(cursor);
            printf("cursor=%i ln=%i begin=%i end=%i length=%i width=%i\n",
                cursor, cursor_ln, line_begin(cursor_ln), line_end(cursor_ln),
                line_end(cursor_ln) - line_begin(cursor_ln), line_width(cursor_ln));
        } break;
    //case ALLEGRO_KEY_TAB: string.append('\t'); break;
    case ALLEGRO_KEY_ESCAPE: break;
    default:
        if (unichar > 0) {
            insert(cursor++, unichar);
        } break;
    }
}

// parses the character array to get metadata about lines; where they begin and end
void TextEditor::parse(int from, int to) {
    assert(font);
    if (from < 0) from += lines.size();
    if (to < 0) to += lines.size();
    lines.clear();

    const int width = bounds.width - padding.x * 2;
    int prev_ch = ALLEGRO_NO_KERNING;
    int line_width=0, line_begin=0,
        word_width=0, word_begin=0;

    for (int i=0; i < chars.size(); i++) {
        int ch = chars[i];
        int advance = al_get_glyph_advance(font, prev_ch, ch);
        // if end of word
        if (isspace(ch)) {
            // if end of line
            if (line_width + word_width + advance > width) {
                lines.emplace_back(line_begin, word_begin-1, line_width);
                // prepare next line
                line_width=word_width;
                line_begin=word_begin;
            } else line_width += word_width + advance;
            // prepare next word
            word_width=0;
            word_begin=i+1;
        } else word_width += advance;
        prev_ch = ch;
    }
}

// moving cursor up and down needs to account for kerning and alignment
bool TextEditor::go_to_line(int ln) {
    assert(font);
    if (ln < 0) ln += line_count();
    if (ln < 0 || ln >= line_count()) return false;

    const int cursor_ln = line_number(cursor);
    int offset = 0; // horizontal distance in pixels of the cursor from point of alignment
    int begin = line_begin(cursor_ln),
        end = line_end(cursor_ln);
    bool left_of_center = false;
    assert(begin < end);

    switch (align.x) {
    case Align::LEFT:
        end = cursor;
        break;
    case Align::RIGHT:
        begin = cursor+1;
        break;
    case Align::CENTER_X: {
            int center = begin + (end - begin) / 2;
            if (cursor < center) {
                left_of_center = true;
                begin = cursor+1;
                end = center;
            } else {
                begin = center+1;
                end = cursor;
            }
        } break;
    }
    
    assert(begin >= 0 && begin < chars.size());
    assert(end >= 0 && end < chars.size());
    int prev_ch = begin > line_begin(cursor_ln) ? chars[begin - 1] : ALLEGRO_NO_KERNING;
    for (int i = begin; i <= end; i++) {
        int ch = chars[i];
        if (ch < 0) continue;
        offset += al_get_glyph_advance(font, prev_ch, ch);
        prev_ch = ch;
    }

    begin = line_begin(ln);
    end = line_end(ln);
    prev_ch = ALLEGRO_NO_KERNING;
    cursor = begin;

    switch (align.x) {
    case Align::RIGHT:
        offset = line_width(ln) - offset;
        break;
    case Align::CENTER_X:
        if (left_of_center)
            offset = line_width(ln) / 2 - offset;
        else offset = line_width(ln) / 2 + offset;
        break;
    }

    for (int i = begin; i <= end && offset > 0; i++) {
        int ch = chars[i];
        if (ch < 0) continue;
        int advance = al_get_glyph_advance(font, prev_ch, ch);
        if (advance > offset) {
            if (advance / 2 > offset) cursor = i;
            break;
        }
        offset -= advance;
        cursor = i;
        prev_ch = ch;
    }

    return true;
}

