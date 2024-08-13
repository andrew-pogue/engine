#pragma once
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <memory>

// wrapper class for ALLEGRO_FONT
class Font {
private:

    std::shared_ptr<ALLEGRO_FONT> font_;

public:

    const char *path;
    const int size, flags;
    
    Font(const char *path, int size, int flags=0) 
        : font_(al_load_font(path, size, flags), al_destroy_font)
        , path(path), size(size), flags(flags)
    { 
        if (!font_) {
            printf("Failed to load font: %s\n");
            (*this) = al_create_builtin_font();
        }
        assert(font_ && "[Font] failed to create a font");
    }

    void operator=(ALLEGRO_FONT *other) { font_.reset(other, al_destroy_font); }

    bool operator==(const ALLEGRO_FONT *other) const { return font_.get() == other; }
    bool operator==(const Font &other) const { return font_ == other.font_; }
    bool operator!=(const ALLEGRO_FONT *other) const { return !(*this == other); }
    bool operator!=(const Font &other) const { return !(*this == other); }

    int get_line_height() const { return al_get_font_line_height(font_.get()); }
    ALLEGRO_FONT *get() { return font_.get(); }

};

