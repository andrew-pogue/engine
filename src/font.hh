#pragma once
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <memory>
#include <string>
#include "color.hh"

// wrapper class for ALLEGRO_FONT
struct Font {

    // size only matters if font is ttf or otf
    Font(std::string path, int size, int flags=0) 
        : font_(al_load_font(path.data(), size, flags), al_destroy_font)
    {
        assert(al_is_font_addon_initialized());
        assert(font_ && "failed to load font");
        // TODO: send error to log file if failed to load font
    }

    void operator=(ALLEGRO_FONT *other) { font_.reset(other, al_destroy_font); }

    bool operator==(const ALLEGRO_FONT *other) const { return font_.get() == other; }
    bool operator!=(const ALLEGRO_FONT *other) const { return !(*this == other); }

    operator bool() const { return (bool)font_; }
    operator ALLEGRO_FONT *() { return font_.get(); }
    operator const ALLEGRO_FONT *() const { return font_.get(); }

    /// Unnecessary functions that just call allegro functions:

    auto line_height() const { return al_get_font_line_height(font_.get()); }
    auto ascent() const { return al_get_font_ascent(font_.get()); }
    auto descent() const { return al_get_font_descent(font_.get()); }

private:

    std::shared_ptr<ALLEGRO_FONT> font_;

};

