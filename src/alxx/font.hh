#pragma once
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

namespace alxx {

class Font {
public:

    Font(const Font &other) = delete;

    Font(const char *filename, int size, int flags)
        : font(al_load_font(filename, size, flags))
        , filename(filename)
        , size(size)
        , flags(flags)
    {
        if (!font) throw "Failed to create font.";
    }

    Font(Font &&other) 
        : font(other.font)
        , size(other.size)
        , flags(other.flags)
    {
        other.font = nullptr;
    }

    ~Font() {
        if (font) al_destroy_font(font);
    }

    ALLEGRO_FONT *get() { return font; }
    const ALLEGRO_FONT *get() const { return font; }

private:

    Font()
        : font(al_create_builtin_font())
        , size(1)
        , flags(0)
    {
        if (!font) throw "Failed to create built-in font.";
    }

    ALLEGRO_FONT *font;
    const char *filename;
    const int size;
    const int flags;

};

} // namespace alxx