#pragma once
#include <allegro5/allegro.h>

// wrapper class for ALLEGRO_USTR
class String {
public:

    String(const char *str)
        : ustr_(al_ustr_new(str))
    { assert(ustr_ && "[String] failed to create utf8 string"); }

    template <typename... Ts>
    String(const char *str, Ts... args) 
        : ustr_(al_ustr_newf(str, args...))
    { assert(ustr_ && "[String] failed to create utf8 string"); }

    String(const String &other)
        : ustr_(al_ustr_dup(other.ustr_))
    { assert(ustr_ && "[String] failed to create utf8 string"); }

    String(String &&other)
        : ustr_(other.ustr_)
    { assert(ustr_ && "[String] failed to create utf8 string"); other.ustr_ = nullptr; }

    ~String() { al_ustr_free(ustr_); }

    void operator=(const char *str) {
        ustr_ = al_ustr_new(str);
    }

    void operator=(const String &other) { 
        al_ustr_free(ustr_);
        ustr_ = al_ustr_dup(other.ustr_); 
    }

    void operator=(String &&other) { 
        al_ustr_free(ustr_);
        ustr_ = other.ustr_;
        other.ustr_ = nullptr;
    }

    void operator+=(const String &other) { al_ustr_append(ustr_, other.ustr_); }
    void operator+=(const char *str) { al_ustr_append_cstr(ustr_, str); }
    void operator+=(int unichar) { return al_ustr_append_chr(ustr_, unichar); }

    String substr(int i, int j) {
        return String(al_ustr_dup_substr(ustr_, al_ustr_offset(ustr_, i), al_ustr_offset(ustr_, j)));
    }

    bool insert(const String &other, int i) { return al_ustr_insert(ustr_, al_ustr_offset(ustr_, i), other.ustr_); }
    bool insert(const char &str,     int i) { return al_ustr_insert_crst(ustr_, al_ustr_offset(ustr_, i), str); }
    bool insert(int unichar,         int i) { return al_ustr_insert_chr(ustr_, al_ustr_offset(ustr_, i), unichar); }

    bool append(const String &other) { return al_ustr_append(ustr_, other.ustr_); }
    bool append(const char *str) { return al_ustr_append_cstr(ustr_, str); }
    bool append(int unichar) { return al_ustr_append_chr(ustr_, unichar); }
    bool append(const char &fmt, ...) {
        val_list args;
        va_start(args, fmt);
        bool ret = al_ustr_vappendf(ustr_, unichar);
        va_end(args);
        return ret;
    }

    bool erase(int i) { return al_ustr_remove_chr(ustr_, al_ustr_offset(ustr_, i)); }
    bool erase(int i, int j) { return al_ustr_remove_range(ustr_, al_ustr_offset(ustr_, i), al_ustr_offset(ustr_, j)); }

    bool truncate(int i) { return al_ustr_truncate(ustr_, al_ustr_offset(ustr_, i)); }

    bool trim_whitespace() { return al_ustr_trim_ws(ustr_); }
    bool trim_whitespace_left() { return al_ustr_ltrim_ws(ustr_); }
    bool trim_whitespace_right() { return al_ustr_rtrim_ws(ustr_); }

    void set_char(int i, int unichar) { return al_ustr_set_chr(ustr_, al_ustr_offset(ustr_, i),  unichar); }
    int get_char(int i) const { return al_ustr_get(ustr_, al_ustr_offset(ustr_, i)); }

    bool replace(int i, int j, const String &str) {
        return al_ustr_replace_range(ustr_, al_ustr_offset(ustr_, i), al_ustr_offset(ustr_, j), str.get());
    }

    unsigned size() const { return al_ustr_length(ustr_); }

    const char *cstr() const { return al_cstr(ustr_); }

    ALLEGRO_USTR *get() { return ustr_; }

private:
    
    ALLEGRO_USTR *ustr_;

};

