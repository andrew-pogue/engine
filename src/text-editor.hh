#pragma once
#include <allegro5/allegro5.h>
#include <cassert>
#include <vector>
#include "layout.hh"
#include "string.hh"
#include "text-effect.hh"
#include "widget.hh"
#include "vector2.hh"
    
// TODO:
// blinking caret
// write tests
// support for text effects
// insertion mode (typing replaces character after caret)
// mouse click changes caret index
// can select multiple characters via mouse
// insert c-string or u-string at index
// copy and paste
// search and replace
// undo and redo
// customizable keybindings / keymaps

struct TextEditor : Widget {

    // These values can be changed without needed to re-parse the text:

    ALLEGRO_COLOR color;
    float scroll;
    int spacing;
    Align align;
    
    TextEditor(Rectangle area, const ALLEGRO_FONT *font, ALLEGRO_COLOR color, const String &string);

    void string(const String &string);
    String string() const;

    const ALLEGRO_FONT *font() const { return font_; }
    void font(const ALLEGRO_FONT *font);

    Vector2<int> padding() const { return padding_; }
    void padding(Vector2<int> val);
    void padding(int x, int y) { padding({x, y}); }

    void render() const override;
    bool handle_event(const ALLEGRO_EVENT &event) override;

    constexpr int word_count() const noexcept { return word_count_; }
    constexpr int line_count() const noexcept { return lines_.size(); }
    constexpr int char_count() const noexcept { return chars_.size() - 1; }
    // get the index of the start of the line with the given line number
    int line_begin(int ln) const;
    // get the index of the end of the line with the given line number
    int line_end(int ln) const;
    // get the line number of the character at the give index
    int line_number(int i) const;
    // get the width in pixels of the line
    int line_width(int ln) const;

    auto cbegin() const { return chars_.cbegin(); }
    auto cend() const { return chars_.cend(); }

    auto begin() { return chars_.begin(); }
    auto end() { return chars_.end(); }

    bool insert(int i, int ch);
    bool remove(int i);

    // void append(int ch);
    // bool remove(int from, int to);
    // bool insert(int i, const char *cstr);
    // bool insert(int i, const ALLEGRO_USTR *ustr);
    // String get_line(int ln) const;
    
    int operator[](int i) const noexcept {
        assert(i >= 0 && i < chars_.size());
        return chars_[i];
    }

    int &operator[](int i) noexcept {
        assert(i >= 0 && i < chars_.size());
        return chars_[i];
    }

    int at(int i) const { return chars_.at(i); }
    int &at(int i) { return chars_.at(i); }

    bool go_to_line(int ln);
    bool go_to(int i);

    int find_next(int ch) const;
    int find_last(int ch) const;
    int find_word_start(int index) const;
    int find_word_end(int index) const;
    int find_index(float x, float y) const;

private:

    struct Cursor { int index, offset; };
    struct LineInfo { int begin, end, width; };

    const ALLEGRO_FONT *font_; // non-owning
    Vector2<int> padding_;
    // each int represents a unicode character
    std::vector<int> chars_;
    // the begin and end of each line, needed for vertical caret movement
    std::vector<LineInfo> lines_;
    Cursor caret_;
    int word_count_;

    // handle keyboard input with no modifier key pressed
    void handle_input(int key, int ch);
    // handle keyboard input with the control modifier key pressed
    void handle_input_ctrl(int key, int ch);

    void parse(int from=0, int to=-1);
    
    // Gets the width in pixels between the start of the character at the index
    // and the point of alignment. The point of alignment can be the beginning of the line [Align::LEFT],
    // the middle of the line [Align::CENTER_X], or the end of the line [Align::RIGHT]. Used for
    // vertical caret movement.
    int offset_from_alignment(int i) const;

    void viewport_update();
    float viewport_begin() const;
    float viewport_end() const;

};

