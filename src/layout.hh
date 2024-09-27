#pragma once
#include <concepts>
#include "vector2.hh"
#include "rectangle.hh"

enum : int {
    ANCHOR_CENTER,
    ANCHOR_NORTH, ANCHOR_SOUTH,
    ANCHOR_EAST,  ANCHOR_WEST,
    ANCHOR_NORTHEAST, ANCHOR_NORTHWEST,
    ANCHOR_SOUTHEAST, ANCHOR_SOUTHWEST,
};

enum : int {
    ALIGN_LEFT,
	ALIGN_TOP = ALIGN_LEFT,
	ALIGN_RIGHT,
	ALIGN_BOTTOM = ALIGN_RIGHT,
	ALIGN_CENTER,
};

struct GridLayout;

struct Layout : Rectangle {

    GridLayout grid(int rows, int columns, Vector2<float> spacing={0.f, 0.f}) const;
    void fill(std::derived_from<Rectangle> auto &...items) const;
    void fill_horizontal(std::derived_from<Rectangle> auto &...items) const;
    void fill_vertical(std::derived_from<Rectangle> auto &...items) const;
    float row(float spacing, std::derived_from<Rectangle> auto &...items) const;
    float column(float spacing, std::derived_from<Rectangle> auto &...items) const;
    void align_horizontal(char value, std::derived_from<Rectangle> auto &...items) const;
    void align_vertical(char value, std::derived_from<Rectangle> auto &...items) const;
    void align(char xalign, char yalign, std::derived_from<Rectangle> auto &...items) const;
    void anchor(char value, std::derived_from<Rectangle> auto &...items) const;
        
}; // struct Layout

struct GridLayout : Rectangle {

    int rows, columns;
    Vector2<float> spacing;

    GridLayout(Rectangle area, int columns, int rows, Vector2<float> spacing={0.f, 0.f});
    Layout at(int gx, int gy) const;
    Layout area(int gx, int gy, int gw, int gh) const;

}; // struct GridLayout

