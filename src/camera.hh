#pragma once

#include "range3.hh"
#include "vector3.hh"

// Represents the players view in game.
// Can translate the coord of an entity to its correct coord on the display.
// Can determine which entities need to be rendered.
// Can "follow" an entity if the viewports coord is updated whenever the entity moves.
class Camera {
public:

    // coordinates of the top left corner of the viewport
    Vector3f origin;

    struct Viewport {
        float width, float height, float zoom;
        Range3f range() const {
            return { origin, { origin.x + width / zoom, origin.y + height / zoom, origin.z } };
        }
    } viewport;

    // origin: in-game location of the top left corner of the viewport
    Camera(Vector3f origin, float view_width, float view_height, float zoom=1.0f)
        : origin(origin)
        , viewport{view_width, view_height, zoom}
    {}

    // translates the given in-game coordinate to the corresponding position on the display
    Vector3f translate(Vector3f coord) const { coord -= origin; coord *= zoom; return coord; }

    void center_on(Vector3f coord) { coord.x -= width / 2.0f; coord.y -= height / 2.0f; origin = coord; }

};

