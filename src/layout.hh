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

struct Align {
    enum Horizontal { LEFT, RIGHT, CENTER_X };
    enum Vertical { TOP, BOTTOM, CENTER_Y };
    Horizontal x = LEFT;
    Vertical y = TOP;
};

struct GridLayout;

struct Layout : Rectangle {

    GridLayout grid(int rows, int columns, Vector2<float> spacing={0.f, 0.f}) const;
    void fill(std::derived_from<Rectangle> auto &...items) const;
    void fill_horizontal(std::derived_from<Rectangle> auto &...items) const;
    void fill_vertical(std::derived_from<Rectangle> auto &...items) const;
    float row(float spacing, std::derived_from<Rectangle> auto &...items) const;
    float column(float spacing, std::derived_from<Rectangle> auto &...items) const;
    void align(Align::Horizontal value, std::derived_from<Rectangle> auto &...items) const;
    void align(Align::Vertical value, std::derived_from<Rectangle> auto &...items) const;
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

