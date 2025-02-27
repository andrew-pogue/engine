#include <allegro5/allegro5.h>
#include <allegro5/allegro_primitives.h>
#include <cmath>
#include <iostream>
#include "text-editor.hh"

// NOTE:
// caret is drawn at the front of the character at its index
// the portion of the text after an insertion or deletion needs to be parsed
//      or the line info will be wrong
// the last character in the char array is appended and only exists so that
//      the caret can move one past the end of the string

// TODO:
// update offset on insert and remove
// parse account for to and from

namespace {

    int min(int a, int b) { return a < b ? a : b; }
    int max(int a, int b) { return a > b ? a : b; }
    int clamp(int val, int low, int high) { return min(max(val, low), high); }

}

///////////////////////////////////////////////////////////////////////////////
/// PUBLIC API

TextEditor::TextEditor(Rectangle area, const ALLEGRO_FONT *font, ALLEGRO_COLOR color, const String &str)
    : Widget(area)
    , color(color), scroll{0.f}, spacing{0}
    , align{Align::LEFT, Align::TOP}
    , font_(font), padding_{0, 0}
    , lines_(), chars_(), caret_{0, 0}, word_count_(0)
{
    assert(font_);
    string(str);
}

void TextEditor::font(const ALLEGRO_FONT *font) {
    font_ = font;
    assert(font_);
    parse();
}

void TextEditor::padding(Vector2<int> val) {
    bool reparse = padding_.x != val.x;
    padding_ = val;
    if (reparse) parse();
}

void TextEditor::render() const {
    assert(font_); if (!font_) return;
    int prev_clip_x=0, prev_clip_y=0, prev_clip_w=0, prev_clip_h=0;
    al_get_clipping_rectangle(&prev_clip_x, &prev_clip_y, &prev_clip_w, &prev_clip_h);
    const bool was_drawing_held = al_is_bitmap_drawing_held();

    const int line_height = al_get_font_line_height(font_);
    const float
        min_x = bounds.x + padding_.x,
        min_y = bounds.y + padding_.y,
        max_x = bounds.x + bounds.width - padding_.x,
        max_y = bounds.y + bounds.height - padding_.y;
    al_set_clipping_rectangle(min_x-2.f, min_y-2.f, max_x-min_x+4.f, max_y-min_y+4.f);
    al_hold_bitmap_drawing(true);

    const int caret_ln = line_number(caret_.index);
    const float caret_y = min_y + caret_ln * (line_height + spacing) - scroll;
    al_draw_filled_rectangle(min_x, caret_y, max_x, caret_y + line_height, al_map_rgb(0,0,255));

    const int begin = clamp(viewport_begin(), 0, line_count()-1),
          end = clamp(viewport_end(), 0, line_count()-1);
    for (int ln = begin; ln <= end; ln++) {
        const float y = min_y + ln * (line_height + spacing) - scroll;
        float x = min_x;
        if (align.x == Align::RIGHT)    x = max_x - line_width(ln);
        if (align.x == Align::CENTER_X) x = max_x - line_width(ln) / 2;
        int prev_ch = ALLEGRO_NO_KERNING;
        for (int i = line_begin(ln); i <= line_end(ln); i++) {
            int ch = chars_.at(i);
            x += al_get_glyph_advance(font_, prev_ch, ch);
            al_draw_glyph(font_, color, x, y, ch);
            if (i == caret_.index)
                al_draw_filled_rectangle(
                    x - 2.f, y - 2.f, x + 2.f, y + line_height + 2.f,
                    al_map_rgb(255,255,255));
            prev_ch = ch;
        }
    }

    al_hold_bitmap_drawing(was_drawing_held);
    al_set_clipping_rectangle(prev_clip_x, prev_clip_y, prev_clip_w, prev_clip_h);
}

bool TextEditor::handle_event(const ALLEGRO_EVENT &event) {
    bool is_handled = false;
    switch(event.type) {
    case ALLEGRO_EVENT_KEY_CHAR:
        handle_input(event.keyboard.keycode, event.keyboard.modifiers, event.keyboard.unichar);
        is_handled = true; break;
    case ALLEGRO_EVENT_MOUSE_AXES:
        scroll += float(20 * event.mouse.dz);
        is_handled = true; break;
    }
    return is_handled;
}

int TextEditor::line_begin(int ln) const {
    if (ln < 0) ln += lines_.size();
    int begin = lines_.at(ln).begin;
    assert(begin >= 0 && begin <= char_count());
    return begin;
}

int TextEditor::line_end(int ln) const {
    if (ln < 0) ln += lines_.size();
    int end = lines_.at(ln).end;
    assert(end >= 0 && end <= char_count());
    return end;
}

int TextEditor::line_number(int i) const {
    i += char_count() * int(i < 0);
    assert(i >= 0 && i <= char_count());
    int ln = 0;
    while (ln < line_count() && i > line_end(ln)) ln++;
    assert(ln >= 0 && ln < lines_.size());
    return ln;
}

int TextEditor::line_width(int ln) const {
    if (ln < 0) ln += lines_.size();
    return lines_.at(ln).width;
}

bool TextEditor::insert(int i, int ch) {
    i += char_count() * int(i < 0);
    if (i < 0 || i > char_count()) return false;
    chars_.insert(chars_.begin() + i, ch);
    parse(line_number(i));
    return true;
}

bool TextEditor::remove(int i) {
    i += char_count() * int(i < 0);
    if (i < 0 || i >= char_count()) return false;
    int ln = line_number(i);
    chars_.erase(chars_.begin() + i);
    parse(ln-1);
    return true;
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

void TextEditor::string(const String &str) {
    chars_.reserve(str.length());
    for (int ch : str) chars_.push_back(ch);
    chars_.push_back(0);
    parse();
}

String TextEditor::string() const { 
    String str;
    for (int ch : chars_) str.append(ch);
    str.truncate(-1);
    return str;
}

bool TextEditor::go_to(int i) {
    i += char_count() * int(i < 0);
    if (i < 0 && i > char_count()) return false;
    caret_.index = i;
    caret_.offset = offset_from_alignment(caret_.index);
    viewport_update();
    return true;
}

// moving caret up and down needs to account for kerning and alignment
bool TextEditor::go_to_line(int ln) {
    assert(font_); if (!font_) return false;
    if (ln < 0) ln += line_count();
    if (ln < 0 || ln >= line_count()) return false;

    int begin = line_begin(ln),
        end = line_end(ln);

    int offset = caret_.offset;
    if (align.x == Align::RIGHT) offset = line_width(ln) + offset;
    else if (align.x == Align::CENTER_X) offset = line_width(ln) / 2 + offset;

    caret_.index = begin;
    int prev_ch = ALLEGRO_NO_KERNING;
    for (int i = begin; i <= end && offset > 0; i++) {
        int ch = chars_[i];
        if (ch < 0) continue;
        int advance = al_get_glyph_advance(font_, prev_ch, ch);
        if (advance > offset) {
            if (advance / 2 > offset) caret_.index = i;
            break;
        }
        offset -= advance;
        caret_.index = i;
        prev_ch = ch;
    }

    viewport_update();
    return true;
}

///////////////////////////////////////////////////////////////////////////////
/// PRIVATE API

void TextEditor::viewport_update() {
    const int ln = line_number(caret_.index);
    const float vp_begin = viewport_begin();
    const int begin = std::ceil(vp_begin); // first line that is fully visible
    if (ln < begin) {
        scroll = ln * (al_get_font_line_height(font_) + spacing);
        return;
    }

    const float vp_end = viewport_end();
    const int end = vp_end - 1.f; // last line that is fully visible

    if (ln > end) {
        const float vp_length = vp_end - vp_begin;
        scroll = (ln + 1 - vp_length) * (al_get_font_line_height(font_) + spacing) - spacing;
    }
}

// can be negative (value of -8 means the viewport starts 8 lines before line 0)
float TextEditor::viewport_begin() const {
    return scroll / (al_get_font_line_height(font_) + spacing);
}

// can be greater than line count
float TextEditor::viewport_end() const {
    const int line_height = al_get_font_line_height(font_);
    const float viewport_height = bounds.height - padding_.y * 2;
    return (scroll + viewport_height) / (line_height + spacing);
}

void TextEditor::handle_input(int key, unsigned modifiers, int ch) {
    switch(key) {
    case ALLEGRO_KEY_UP: {
            int ln = line_number(caret_.index);
            if (ln > 0) go_to_line(ln-1);
        } break;
    case ALLEGRO_KEY_DOWN: {
            int ln = line_number(caret_.index);
            if (ln < line_count()-1) go_to_line(ln+1);
        } break;
    case ALLEGRO_KEY_LEFT:
        if (caret_.index > 0) {
            int to = caret_.index - 1;
            if (modifiers & ALLEGRO_KEYMOD_CTRL) {
                // jump one word to the left
                while (to > 0 && isspace(chars_[to])) to--;
                while (to > 0 && !isspace(chars_[to-1])) to--;
                //to += int(isspace(chars_[to]) == true);
            }
            bool success = go_to(to);
            assert(success);
        } break;
    case ALLEGRO_KEY_RIGHT:
        if (caret_.index < char_count()) {
            int to = caret_.index + 1;
            if (modifiers & ALLEGRO_KEYMOD_CTRL) {
                // jump one word to the right
                while (to < char_count() && isspace(chars_[to])) to++;
                while (to < char_count() && !isspace(chars_[to])) to++;
            }
            bool success = go_to(to);
            assert(success);
        } break;
    case ALLEGRO_KEY_BACKSPACE:
        if (caret_.index > 0) {
            bool success = remove(--caret_.index);
            assert(success);
        } break;
    case ALLEGRO_KEY_DELETE:
        if (caret_.index >= 0 && caret_.index < char_count()) {
            bool success = remove(caret_.index);
            assert(success);
        } break;
    //case ALLEGRO_KEY_ENTER: insert(caret_.index++, '\n'); break;
    //case ALLEGRO_KEY_TAB: string.append('\t'); break;
    case ALLEGRO_KEY_HOME: {
            bool success = go_to(line_begin(line_number(caret_.index)));
            assert(success);
        } break;
    case ALLEGRO_KEY_END: {
            bool success = go_to(line_end(line_number(caret_.index)));
            assert(success);
        } break;
    case ALLEGRO_KEY_PGUP: {
            int from = line_number(caret_.index),
                to = from - int(viewport_end() - viewport_begin());
            if (to < 0) to = 0;
            bool success = go_to_line(to);
            assert(success);
        } break;
    case ALLEGRO_KEY_PGDN: {
            int from = line_number(caret_.index),
                to = from + int(viewport_end() - viewport_begin());
            if (to > line_count()-1) to = line_count()-1;
            bool success = go_to_line(to);
            assert(success);
        } break;
    case ALLEGRO_KEY_ESCAPE: break;
    default:
        if (ch > 0) {
            insert(caret_.index++, ch);
        } break;
    }
}

// parses the character array to get metadata about lines; their width, where they begin and end
void TextEditor::parse(int from, int to) {
    assert(font_); if (!font_) return;
    from += line_count() * (from < 0);
    to += line_count() * (to < 0);
    lines_.clear();

    // if adding a word to a line extends its width beyond the max width, start new line
    // whitespace characters delimiting lines don't contribute to line width
   
    const int max_width = bounds.width - padding_.x * 2;
    int prev_ch = ALLEGRO_NO_KERNING;
    int line_begin = 0, word_begin = 0;
    int delimiter_width = 0, line_width = 0, word_width = 0;

    for (int i=0; i <= char_count(); i++) {
        int ch = chars_[i];

        if (isspace(prev_ch)) delimiter_width = al_get_glyph_advance(font_, prev_ch, ch);
        else word_width += al_get_glyph_advance(font_, prev_ch, ch);

        if (isspace(ch)) {
            word_count_ += int(word_width > 0);
            // if end of line
            if (line_width + delimiter_width + word_width > max_width) {
                lines_.emplace_back(line_begin, word_begin-1, line_width);
                // prepare next line
                line_width = word_width;
                line_begin = word_begin;
                delimiter_width = 0;
            } else line_width += delimiter_width + word_width;
            if (ch == '\n') {
                lines_.emplace_back(line_begin, i, line_width);
                // prepare next line
                line_width = 0;
                line_begin = i+1;
                delimiter_width = 0;
            }
            // prepare next word
            word_width = 0;
            word_begin = i+1;
        }
        prev_ch = ch;
    }
    lines_.emplace_back(line_begin, char_count(), line_width);
    
    int caret_ln = line_number(caret_.index);
    if (caret_ln >= from && caret_ln <= to)
        caret_.offset = offset_from_alignment(caret_.index);
}

int TextEditor::offset_from_alignment(int i) const {
    assert(font_); if (!font_) return 0;
    if (i < 0) i += char_count();
    assert(i >= 0 && i <= char_count());

    const int ln = line_number(i);
    int offset = 0; 
    int begin = line_begin(ln),
        end = line_end(ln);
    bool negate = false;
    assert(begin <= end);

    switch (align.x) {
    case Align::LEFT:
        end = i;
        break;
    case Align::RIGHT:
        begin = i+1;
        negate = true;
        break;
    case Align::CENTER_X: {
            int center = begin + (end - begin) / 2;
            if (i < center) {
                begin = i+1;
                end = center;
                negate = true;
            } else {
                begin = center+1;
                end = i;
            }
        } break;
    }
    
    assert(begin >= 0 && begin <= char_count());
    assert(end >= 0 && end <= char_count());
    int prev_ch = begin > line_begin(ln) ? chars_[begin - 1] : ALLEGRO_NO_KERNING;
    for (int i = begin; i <= end; i++) {
        int ch = chars_[i];
        if (ch < 0) continue;
        offset += al_get_glyph_advance(font_, prev_ch, ch);
        prev_ch = ch;
    }

    return negate ? -offset : offset;
}

