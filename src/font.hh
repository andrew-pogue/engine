#pragma once
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

// wrapper class for ALLEGRO_FONT
class Font {
private:

    ALLEGRO_FONT *const font_;

public:

    const char *path;
    const int size, flags;
    
    Font(const Font &other) = delete;
   
    Font(const char *path, int size, int flags=0) 
        : font_(al_load_font(path, size, flags))
        , path(path), size(size), flags(flags)
    { 
        if (!font_) {
            printf("Failed to load font: %s\n");
            font_ = al_create_builtin_font();
        }
        assert(font_ && "[Font] failed to create a font");
    }

    Font(Font &&other) 
        : font(other.font)
        , size(other.size)
        , flags(other.flags)
    {
        if (!font_) font_ = al_create_builtin_font();
        assert(font_ && "[Font] failed to create a font");
    }

    ~Font() { al_destroy_font(font_); }

    void operator=(const Font &other) = delete;
    void operator=(ALLEGRO_FONT *other) {
        al_destroy_font(font_);
        font_ = other;
    }
    void operator=(ALLEGRO_FONT &&other) {
        al_destroy_font(font_);
        font_ = other.font_;
    }

    bool opeartor==(const ALLEGRO_FONT *other) const { return font_ == other; }
    bool opeartor==(const Font &other) const { return font_ == other.font_; }
    bool opeartor!=(const ALLEGRO_FONT *other) const { return !(*this == other); }
    bool opeartor!=(const Font &other) const { return !(*this == other); }

    ALLEGRO_FONT *get() { return font_; }

};

