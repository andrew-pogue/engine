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

void Layout::align(Align::Horizontal value, std::derived_from<Rectangle> auto &...items) const {
    for (Rectangle *item : { &items... }) {
        switch (value) {
        case Align::LEFT:       break;
        case Align::CENTER_X:   item->x = x + (width - item->width) / 2.0f; break;
        case Align::RIGHT:      item->x = x +  width - item->width; break;
        }
    }
}

void Layout::align(Align::Vertical value, std::derived_from<Rectangle> auto &...items) const {
    for (Rectangle *item : { &items... }) {
        switch (value) {
        case Align::TOP:        break;
        case Align::CENTER_Y:   item->y = y + (height - item->height) / 2.0f; break;
        case Align::BOTTOM:     item->y = y +  height - item->height; break;
        }
    }
}

void Layout::align(Align value, std::derived_from<Rectangle> auto &...items) const {
    align(value.x, items...); align(value.y, items...);
}

void Layout::anchor(char value, std::derived_from<Rectangle> auto &...items) const {
    switch (value) {
    case Anchor::NORTH:     align(Align::CENTER_X,  Align::TOP,         items...); break;
    case Anchor::NORTHEAST: align(Align::RIGHT,     Align::TOP,         items...); break; 
    case Anchor::NORTHWEST: align(Align::LEFT,      Align::TOP,         items...); break;
    case Anchor::SOUTH:     align(Align::CENTER_X,  Align::BOTTOM,      items...); break;
    case Anchor::SOUTHEAST: align(Align::RIGHT,     Align::BOTTOM,      items...); break;
    case Anchor::SOUTHWEST: align(Align::LEFT,      Align::BOTTOM,      items...); break;
    case Anchor::WEST:      align(Align::LEFT,      Align::CENTER_Y,    items...); break;
    case Anchor::EAST:      align(Align::RIGHT,     Align::CENTER_Y,    items...); break;
    case Anchor::CENTER:    align(Align::CENTER_X,  Align::CENTER_Y,    items...); break;
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

