#include "layout.hh"

GridLayout Layout::grid(int rows, int columns, Vector2<float> spacing) const {
    return GridLayout(*this, rows, columns, spacing);
}

void Layout::fill(std::derived_from<Rectangle> auto &...items) const {
    for (Rectangle *item : { &items... }) *item = *(Rectangle *)this;
}

void Layout::fill_horizontal(std::derived_from<Rectangle> auto &...items) const {
    for (Rectangle *item : { &items... }) {
        item->x = x;
        item->width = width;
    }
}

void Layout::fill_vertical(std::derived_from<Rectangle> auto &...items) const {
    for (Rectangle *item : { &items... }) {
        item->y = y;
        item->height = height;
    }
}

float Layout::row(float spacing, std::derived_from<Rectangle> auto &...items) const {
    float shift = 0.0f;
    for (Rectangle *item : { &items... }) {
        item->x = x + shift;
        shift += item->width + spacing;
    }
    return shift - spacing;
}

float Layout::column(float spacing, std::derived_from<Rectangle> auto &...items) const {
    float shift = 0.0f;
    for (Rectangle *item : { &items... }) {
        item->y = y + shift;
        shift += item->height + spacing;
    }
    return shift - spacing;
}

void Layout::align_horizontal(char value, std::derived_from<Rectangle> auto &...items) const {
    for (Rectangle *item : { &items... }) {
        switch (value) {
        case ALIGN_LEFT:   break;
        case ALIGN_CENTER: item->x = x + (width - item->width) / 2.0f; break;
        case ALIGN_RIGHT:  item->x = x +  width - item->width; break;
        }
    }
}

void Layout::align_vertical(char value, std::derived_from<Rectangle> auto &...items) const {
    for (Rectangle *item : { &items... }) {
        switch (value) {
        case ALIGN_TOP:    break;
        case ALIGN_CENTER: item->y = y + (height - item->height) / 2.0f; break;
        case ALIGN_BOTTOM: item->y = y +  height - item->height; break;
        }
    }
}

void Layout::align(char xalign, char yalign, std::derived_from<Rectangle> auto &...items) const {
    align_horizontal(xalign, items...);
    align_vertical(yalign, items...);
}

void Layout::anchor(char value, std::derived_from<Rectangle> auto &...items) const {
    switch (value) {
    case ANCHOR_NORTH:     align(ALIGN_CENTER, ALIGN_TOP,    items...); break;
    case ANCHOR_NORTHEAST: align(ALIGN_RIGHT,  ALIGN_TOP,    items...); break; 
    case ANCHOR_NORTHWEST: align(ALIGN_LEFT,   ALIGN_TOP,    items...); break;
    case ANCHOR_SOUTH:     align(ALIGN_CENTER, ALIGN_BOTTOM, items...); break;
    case ANCHOR_SOUTHEAST: align(ALIGN_RIGHT,  ALIGN_BOTTOM, items...); break;
    case ANCHOR_SOUTHWEST: align(ALIGN_LEFT,   ALIGN_BOTTOM, items...); break;
    case ANCHOR_WEST:      align(ALIGN_LEFT,   ALIGN_CENTER, items...); break;
    case ANCHOR_EAST:      align(ALIGN_RIGHT,  ALIGN_CENTER, items...); break;
    case ANCHOR_CENTER:    align(ALIGN_CENTER, ALIGN_CENTER, items...); break;
    }
}

GridLayout::GridLayout(Rectangle area, int columns, int rows, Vector2<float> spacing)
    : Rectangle(area)
    , rows(rows), columns(columns)
    , spacing(spacing)
{}

Layout GridLayout::at(int gx, int gy) const {
    const float tw = (width - (columns + 1) * spacing.x) / columns;
    const float th = (height - (rows + 1) * spacing.y) / rows;
    return {
        x + spacing.x + float(gx) * (tw + spacing.x),
        y + spacing.y + float(gy) * (th + spacing.y),
        tw , th
    };
}

Layout GridLayout::area(int gx, int gy, int gw, int gh) const {
    const float tw = (width - (columns + 1) * spacing.x) / columns;
    const float th = (height - (rows + 1) * spacing.y) / rows;
    return {
        x + spacing.x + float(gx) * (tw + spacing.x),
        y + spacing.y + float(gy) * (th + spacing.y),
        float(gw) * (tw + spacing.x) - spacing.x,
        float(gh) * (th + spacing.y) - spacing.y
    };
}

