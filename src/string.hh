#pragma once
#include <allegro5/allegro.h>

// wrapper class for ALLEGRO_USTR
struct String {

    String()
        : ustr_(al_ustr_new(""))
    { assert(ustr_ && "[String] failed to create utf8 string"); }

    String(ALLEGRO_USTR *ustr)
        : ustr_(ustr)
    { assert(ustr_ && "[String] failed to create utf8 string"); }

    String(const char *cstr)
        : ustr_(al_ustr_new(cstr))
    { assert(ustr_ && "[String] failed to create utf8 string"); }

    template <typename... Ts>
    String(const char *cstr, Ts... args) 
        : ustr_(al_ustr_newf(cstr, args...))
    { assert(ustr_ && "[String] failed to create utf8 string"); }

    String(String &&other)
        : ustr_(other.ustr_)
    { assert(ustr_ && "[String] failed to create utf8 string"); other.ustr_ = nullptr; }

    String(const String &other)
        : ustr_(al_ustr_dup(other.ustr_))
    { assert(ustr_ && "[String] failed to create utf8 string"); }

    ~String() { al_ustr_free(ustr_); }

    void operator=(ALLEGRO_USTR *ustr) { al_ustr_free(ustr_); ustr_ = ustr; }
    void operator=(const char *cstr) { al_ustr_free(ustr_); ustr_ = al_ustr_new(cstr); }
    void operator=(const String &other) { al_ustr_free(ustr_); ustr_ = al_ustr_dup(other.ustr_); }
    void operator=(String &&other) { al_ustr_free(ustr_); ustr_ = other.ustr_; other.ustr_ = nullptr; }

    void operator+=(const ALLEGRO_USTR *ustr) { append(ustr); }
    void operator+=(const String &other) { append(other); }
    void operator+=(const char *cstr) { append(cstr); }
    void operator+=(int ch) { append(ch); }

    String operator+(const String &other) const { String ret = *this; ret.append(other); return ret; }
    String operator+(const char *cstr) const { String ret = *this; ret.append(cstr); return ret; }
    String operator+(int ch) const { String ret = *this; ret.append(ch); return ret; }

    String substr(int i, int j) {
        return al_ustr_dup_substr(ustr_, al_ustr_offset(ustr_, i), al_ustr_offset(ustr_, j));
    }

    // inserts a *copy* of the ustr onto the end of this string
    bool insert(int i, const String &other) { return al_ustr_insert(ustr_, al_ustr_offset(ustr_, i), other.ustr_); }
    bool insert(int i, const char *cstr) { return al_ustr_insert_cstr(ustr_, al_ustr_offset(ustr_, i), cstr); }
    bool insert(int i, int ch) { return al_ustr_insert_chr(ustr_, al_ustr_offset(ustr_, i), ch); }

    // appends a *copy* of the ustr onto the end of this string
    bool append(const ALLEGRO_USTR *ustr) { return al_ustr_append(ustr_, ustr); }
    bool append(const String &other) { return al_ustr_append(ustr_, other.ustr_); }
    bool append(const char *cstr) { return al_ustr_append_cstr(ustr_, cstr); }
    bool append(int ch) { return al_ustr_append_chr(ustr_, ch); }
    bool appendf(const char *fmt, ...) {
        va_list args;
        va_start(args, fmt);
        bool ret = al_ustr_vappendf(ustr_, fmt, args);
        va_end(args);
        return ret;
    }

    bool erase(int i) { return al_ustr_remove_chr(ustr_, al_ustr_offset(ustr_, i)); }
    bool erase(int i, int j) { return al_ustr_remove_range(ustr_, al_ustr_offset(ustr_, i), al_ustr_offset(ustr_, j)); }

    bool truncate(int i) { return al_ustr_truncate(ustr_, al_ustr_offset(ustr_, i)); }

    bool trim_whitespace() { return al_ustr_trim_ws(ustr_); }
    bool trim_whitespace_left() { return al_ustr_ltrim_ws(ustr_); }
    bool trim_whitespace_right() { return al_ustr_rtrim_ws(ustr_); }

    void set_char(int i, int ch) { al_ustr_set_chr(ustr_, al_ustr_offset(ustr_, i),  ch); }
    int get_char(int i) const { return al_ustr_get(ustr_, al_ustr_offset(ustr_, i)); }

    struct Character {

        Character(ALLEGRO_USTR *ustr, int i)
            : index{i}, ustr{ustr}
        { assert(ustr); }

        void operator=(int ch) { al_ustr_set_chr(ustr, al_ustr_offset(ustr, index), ch); }
        operator int() const { return al_ustr_get(ustr, al_ustr_offset(ustr, index)); }

        bool operator<(int ch) const { return ch < (int)(*this); }
        bool operator>(int ch) const { return ch > (int)(*this); }
        bool operator==(int ch) const { return ch == (int)(*this); }
        bool operator!=(int ch) const { return ch != (int)(*this); }
        bool operator<=(int ch) const { return ch <= (int)(*this); }
        bool operator>=(int ch) const { return ch >= (int)(*this); }

    private:

        int index;
        ALLEGRO_USTR *ustr; // non-owning
    
    }; // struct Character

    const Character operator[](int i) const { return Character(ustr_, i); }
          Character operator[](int i)       { return Character(ustr_, i); }

    bool replace(int i, int j, const String &str) {
        return al_ustr_replace_range(ustr_, al_ustr_offset(ustr_, i), al_ustr_offset(ustr_, j), str.get());
    }

    unsigned length() const { return al_ustr_length(ustr_); }

    const char *cstr() const { return al_cstr(ustr_); }

          ALLEGRO_USTR *get()       { return ustr_; }
    const ALLEGRO_USTR *get() const { return ustr_; }

    struct ConstIterator {

        ConstIterator(ALLEGRO_USTR *str, int offset) : str(str), pos(offset) {}

        ConstIterator operator++() { al_ustr_next(str, &pos); return *this; }
        ConstIterator operator--() { al_ustr_prev(str, &pos); return *this; }
        ConstIterator operator++(int) { auto tmp = *this; ++(*this); return tmp; }
        ConstIterator operator--(int) { auto tmp = *this; --(*this); return tmp; }

        bool operator==(const ConstIterator &other) const { return other.str == str && other.pos == pos; }
        bool operator!=(const ConstIterator &other) const { return !(other == *this); }

        int operator*() const { return al_ustr_get(str, pos); }

    private:

        ALLEGRO_USTR *str; // non-owning
        int pos;

    }; // struct ConstIterator

    ConstIterator begin() const { return ConstIterator(ustr_, al_ustr_offset(ustr_, 0)); }
    ConstIterator end() const { return ConstIterator(ustr_, al_ustr_size(ustr_)); }

private:
    
    ALLEGRO_USTR *ustr_;

};

