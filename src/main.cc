#include <allegro5/allegro.h>
#include <allegro5/allegro_color.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <cmath>
#include <memory>

#include "actor.hh"
#include "context.hh"
#include "grid2.hh"
#include "keyboard.hh"
#include "layout.hh"
#include "rectangle.hh"
#include "text-effect.hh"
#include "vector2.hh"

enum Tile : unsigned {
    TILE_GRASSLAND,
    TILE_MOUNTAIN,
    TILE_FOREST_PINE,
    TILE_FOREST_DECIDUOUS,
    TILE_HILL,
    TILE_VILLAGE,
    TILE_RIVER,
    TILE_PLAYER
};
const int DISPLAY_WIDTH = 640, DISPLAY_HEIGHT = 480;

ALLEGRO_DISPLAY *create_display() {
    al_set_new_window_title("New Colony");
    return al_create_display(DISPLAY_WIDTH, DISPLAY_HEIGHT);
}

ALLEGRO_BITMAP *load_tileset() {
    return al_load_bitmap("assets/tileset/urizen_onebit.png");
}

Grid2<Tile> load_terrain() {
    Grid2<Tile> terrain(20, 20, TILE_GRASSLAND);
    for (int i = 0; i < 20; i++) {
        terrain.at(i,0) = TILE_MOUNTAIN;
        terrain.at(i,19) = TILE_MOUNTAIN;
    }
    for (int j = 0; j < 20; j++) {
        terrain.at(0,j) = TILE_MOUNTAIN;
        terrain.at(19,j) = TILE_MOUNTAIN;
        terrain.at(12,j) = TILE_RIVER;
    }
    terrain.at(1,1) = TILE_HILL;
    terrain.at(2,1) = TILE_HILL;
    terrain.at(1,2) = TILE_HILL;
    terrain.at(10,10) = TILE_FOREST_PINE;
    terrain.at(5,11) = TILE_FOREST_PINE;
    terrain.at(13,5) = TILE_FOREST_PINE;
    terrain.at(13,9) = TILE_VILLAGE;
    return terrain;
}

// NOTE: creating a font before creating a display will cause issues with allegro
// size is ignored if the font is a bitmap font
ALLEGRO_FONT *load_font(const char *name, const char *style, int size=0) {
    std::string path = "assets/font/";
    path += name; path += '-'; path += style; path += ".ttf";
    return al_load_font(path.data(), size, 0);
}

void draw_tile(ALLEGRO_BITMAP *tileset, Tile tile, float x, float y, float width, float height) {
    int column = 0, row = 0;
    const int tile_size = 12;
    switch (tile) {
    case TILE_FOREST_DECIDUOUS: column = 1; row = 34; break;
    case TILE_FOREST_PINE: column = 2; row = 34; break;
    case TILE_GRASSLAND: column = 12; row = 34; break;
    case TILE_HILL: column = 13; row = 34; break;
    case TILE_MOUNTAIN: column = 15; row = 34; break;
    case TILE_RIVER: column = 0; row = 12; break;
    case TILE_VILLAGE: column = 17; row = 33; break;
    case TILE_PLAYER: column = 26; row = 40; break;
    }
    al_draw_scaled_bitmap(tileset,
        column * (tile_size+1), row * (tile_size+1), tile_size+1, tile_size+1,
        x, y, width, height, 0);
}

void draw_tile(ALLEGRO_BITMAP *tileset, Tile tile, Rectangle area) {
    draw_tile(tileset, tile, area.x, area.y, area.width, area.height);
}

void render(const Grid2<Tile> &terrain, ALLEGRO_BITMAP *tileset,
    Rectangle viewport, Vector2<float> focus, float tile_size, bool draw_grid = false
) {
    int prev_clip_x=0, prev_clip_y=0, prev_clip_w=0, prev_clip_h=0;
    al_get_clipping_rectangle(&prev_clip_x, &prev_clip_y, &prev_clip_w, &prev_clip_h);
    al_set_clipping_rectangle(viewport.x, viewport.y, viewport.width, viewport.height);
    // width and height of the viewport in tiles
    const float width  = viewport.width  / tile_size,
                height = viewport.height / tile_size;
    // tile position of the top left corner of the viewport
    const Vector2<float> origin{ focus.x - width / 2.f, focus.y + height / 2.f };
    // all the tiles that are at least partially visible
    const Vector2<int> min{ int(origin.x), int(origin.y - height) },
                       max{ int(origin.x + width), int(origin.y) };
    for (int x = min.x; x <= max.x; x++) {
        for (int y = min.y; y <= max.y; y++) {
            if (!terrain.contains(x, y)) continue;
            Rectangle area{ viewport.x + (float(x) - origin.x)       * tile_size,
                            viewport.y + (origin.y - float(y) - 1.f) * tile_size,
                            tile_size, tile_size };
            draw_tile(tileset, terrain.at(x, y), area);
            if (draw_grid) area.draw_outline(al_color_name("grey"), -1);
        }
    }
    draw_tile(tileset, TILE_PLAYER,
        viewport.x + (focus.x - origin.x - 0.5) * tile_size,
        viewport.y + (origin.y - focus.y - 0.5) * tile_size,
        tile_size, tile_size);
    al_set_clipping_rectangle(prev_clip_x, prev_clip_y, prev_clip_w, prev_clip_h);
}

void must_init(bool test, const char *error_message) {
    if (test) return;
    std::cerr << error_message << '\n';
    exit(1);
}

int main(int argc, char **argv) {
    (void)argc; (void)argv;
    
    must_init(al_init(), "Failed to initialize allegro.");
    must_init(al_install_keyboard(), "Failed to install keyboard driver.");
    must_init(al_install_mouse(), "Failed to install mouse driver.");
    must_init(al_init_font_addon(), "Failed to initialize font addon.");
    must_init(al_init_ttf_addon(), "Failed to initialize ttf addon.");
    must_init(al_init_primitives_addon(), "Failed to initialize primitives addon.");
    must_init(al_init_image_addon(), "Failed to initialize image addon.");

    ALLEGRO_DISPLAY *display = create_display();
    must_init(display, "Failed to create display.");

    ALLEGRO_EVENT_QUEUE *event_queue = al_create_event_queue();
    must_init(event_queue, "Failed to create event queue.");
    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_register_event_source(event_queue, al_get_mouse_event_source());

    ALLEGRO_BITMAP *tileset = load_tileset();
    must_init(tileset, "Failed to load tileset.");
    const float tile_size = 12.f;

    ALLEGRO_FONT *font = load_font("PressStart2P", "Regular", 16);
    must_init(font, "Failed to load font.");

    Grid2<Tile> terrain = load_terrain();
    GridLayout window{
        { 0.f, 0.f, float(al_get_display_width(display)), float(al_get_display_height(display)) },
        6, 6, {4.f, 4.f}};
    Rectangle viewport = window.area(1,1,4,4);
    Rectangle title = window.area(1,0,4,1);

    Actor player{{9.5f, 9.5f}};
    PlayerContext context(display, &player);
    float scale = 3.0f;

    ALLEGRO_EVENT event;
    bool play = true;
    double time_current = al_get_time(), time_prior = time_current;
    double lag = 0.0, step_size = 1.0 / 60.0;

    /// GAME LOOP /////////////////////////////////////////////////////////////////////
    while (play) {
        double time_elapsed = time_current - time_prior;

        /// HANDLE EVENTS
        while (al_get_next_event(event_queue, &event)) {
            context.handle_event(event);
            switch (event.type) {
            case ALLEGRO_EVENT_DISPLAY_CLOSE:
                play = false;
                break;
            case ALLEGRO_EVENT_MOUSE_AXES:
                scale += event.mouse.dz * 0.1f;
                break;
            /*
            case ALLEGRO_EVENT_DISPLAY_RESIZE:
                if (event.display.source == display) {
                    al_acknowledge_resize(display);
                } break;
            case ALLEGRO_EVENT_DISPLAY_SWITCH_OUT:
                if (event.display.source == display) {
                    if (al_is_keyboard_installed()) al_clear_keyboard_state(display);
                } break;
            */
            }
        }

        /// UPDATE
        play = context.update(time_elapsed);

        /// FIXED UPDATE
        for (lag += time_elapsed; lag >= step_size; lag -= step_size) {
            player.step();
        }

        /// RENDER
        al_clear_to_color(al_map_rgb(0,0,0));
        draw_textbox_with_effects(
            title, {Align::CENTER_X, Align::CENTER_Y}, 2.f, 0.f, font, al_color_name("snow"), "Hello, World!",
            WavyTextEffect(100.f,8.f,time_current*75.0),
            RainbowTextEffect(time_current*100.0));
        render(terrain, tileset, viewport, player.position, tile_size * scale);
        viewport.draw_outline(al_color_name("snow"), -1);
        //al_draw_line(DISPLAY_WIDTH / 2.f, 0.f, DISPLAY_WIDTH / 2.f, DISPLAY_HEIGHT, al_color_name("white"), 0.f);
        //al_draw_line(0.f, DISPLAY_HEIGHT / 2.f, DISPLAY_WIDTH, DISPLAY_HEIGHT / 2.f, al_color_name("white"), 0.f);
        al_flip_display();

        time_prior = time_current;
        time_current = al_get_time();
    }
    ///////////////////////////////////////////////////////////////////////////////////

    al_destroy_font(font);
    al_destroy_bitmap(tileset);
    al_destroy_event_queue(event_queue);
    al_destroy_display(display);

    return 0;
}

