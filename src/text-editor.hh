#pragma once
#include <allegro5/allegro5.h>
#include <cassert>
#include <vector>
#include "layout.hh"
#include "string.hh"
#include "text-effect.hh"
#include "vector2.hh"
#include "widget.hh"
    
// TODO:
// blinking cursor
// write tests
// support for text effects
// insertion mode (typing replaces character after cursor)
// mouse click changes cursor index
// can select multiple characters via mouse
// insert c-string or u-string at index
// copy and paste
// search and replace
// undo and redo

struct TextEditor : Widget {

    const ALLEGRO_FONT *font; // non-owning
    ALLEGRO_COLOR color;
    float scroll, spacing;
    Vector2<float> padding;
    Align align;

    TextEditor(Rectangle area, const ALLEGRO_FONT *font, ALLEGRO_COLOR color, String &&string);

    void render() const override;
    bool handle_event(const ALLEGRO_EVENT &event) override;

    constexpr int word_count() const noexcept { return num_words; }
    constexpr int line_count() const noexcept { return lines.size(); }
    constexpr int char_count() const noexcept { return chars.size(); }
    // get the index of the start of the line with the given line number
    int line_begin(int ln) const;
    // get the index of the end of the line with the given line number
    int line_end(int ln) const;
    // get the line number of the character at the give index
    int line_number(int i) const;
    // get the width in pixels of the line
    int line_width(int ln) const;

    String to_string() const;

    auto cbegin() const { return chars.cbegin(); }
    auto cend() const { return chars.cend(); }

    auto begin() { return chars.begin(); }
    auto end() { return chars.end(); }

    void insert(int i, int ch);
    void remove(int i);

    // void append(int ch);
    // bool remove(int from, int to);
    // bool insert(int i, const char *cstr);
    // bool insert(int i, const ALLEGRO_USTR *ustr);
    // String get_line(int ln) const;
    
    int operator[](int i) const noexcept {
        assert(i >= 0 && i < chars.size());
        return chars[i];
    }

    int &operator[](int i) noexcept {
        assert(i >= 0 && i < chars.size());
        return chars[i];
    }

    int at(int i) const { return chars.at(i); }
    int &at(int i) { return chars.at(i); }

    void go_to_line(int ln);
    void go_to(int i);

private:

    struct Cursor { int index, offset; };
    struct LineInfo { int begin, end, width; };

    // each int represents a unicode character
    std::vector<int> chars;
    // the begin and end of each line, needed for vertical cursor movement
    std::vector<LineInfo> lines;
    Cursor cursor;
    int num_words;

    void handle_input(int keycode, int unichar);
    void parse(int from=0, int to=-1);
    
    // Gets the width in pixels between the start of the character at the index
    // and the point of alignment. The point of alignment can be the beginning of the line [Align::LEFT],
    // the middle of the line [Align::CENTER_X], or the end of the line [Align::RIGHT]. Used for
    // vertical cursor movement.
    int offset_from_alignment(int i) const;

    // int find_index(float x, float y);

};

