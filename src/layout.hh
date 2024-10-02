#pragma once
#include <concepts>
#include "vector2.hh"
#include "rectangle.hh"

enum class Anchor {
    NORTH, SOUTH,
    EAST,  WEST,
    NORTHEAST, NORTHWEST,
    SOUTHEAST, SOUTHWEST,
    CENTER,
};

enum class AlignX { LEFT, RIGHT, CENTER };
enum class AlignY { TOP, BOTTOM, CENTER };

struct Align {
    AlignX x : 2 = AlignX::LEFT;
    AlignY y : 2 = AlignY::TOP;
};

struct GridLayout;

struct Layout : Rectangle {

    GridLayout grid(int rows, int columns, Vector2<float> spacing={0.f, 0.f}) const;
    void fill(std::derived_from<Rectangle> auto &...items) const;
    void fill_horizontal(std::derived_from<Rectangle> auto &...items) const;
    void fill_vertical(std::derived_from<Rectangle> auto &...items) const;
    float row(float spacing, std::derived_from<Rectangle> auto &...items) const;
    float column(float spacing, std::derived_from<Rectangle> auto &...items) const;
    void align(AlignX value, std::derived_from<Rectangle> auto &...items) const;
    void align(AlignY value, std::derived_from<Rectangle> auto &...items) const;
    void align(Align value, std::derived_from<Rectangle> auto &...items) const;
    void anchor(char value, std::derived_from<Rectangle> auto &...items) const;
        
}; // struct Layout

struct GridLayout : Rectangle {

    int rows, columns;
    Vector2<float> spacing;

    GridLayout(Rectangle area, int columns, int rows, Vector2<float> spacing={0.f, 0.f});
    Layout at(int gx, int gy) const;
    Layout area(int gx, int gy, int gw, int gh) const;

}; // struct GridLayout

