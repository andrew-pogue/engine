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

void Layout::align(AlignX value, std::derived_from<Rectangle> auto &...items) const {
    for (Rectangle *item : { &items... }) {
        switch (value) {
        case AlignX::LEFT:   break;
        case AlignX::CENTER: item->x = x + (width - item->width) / 2.0f; break;
        case AlignX::RIGHT:  item->x = x +  width - item->width; break;
        }
    }
}

void Layout::align(AlignY value, std::derived_from<Rectangle> auto &...items) const {
    for (Rectangle *item : { &items... }) {
        switch (value) {
        case AlignY::TOP:    break;
        case AlignY::CENTER: item->y = y + (height - item->height) / 2.0f; break;
        case AlignY::BOTTOM: item->y = y +  height - item->height; break;
        }
    }
}

void Layout::align(Align value, std::derived_from<Rectangle> auto &...items) const {
    align(value.x, items...); align(value.y, items...);
}

void Layout::anchor(char value, std::derived_from<Rectangle> auto &...items) const {
    switch (value) {
    case Anchor::NORTH:     align(AlignX::CENTER, AlignY::TOP,    items...); break;
    case Anchor::NORTHEAST: align(AlignX::RIGHT,  AlignY::TOP,    items...); break; 
    case Anchor::NORTHWEST: align(AlignX::LEFT,   AlignY::TOP,    items...); break;
    case Anchor::SOUTH:     align(AlignX::CENTER, AlignY::BOTTOM, items...); break;
    case Anchor::SOUTHEAST: align(AlignX::RIGHT,  AlignY::BOTTOM, items...); break;
    case Anchor::SOUTHWEST: align(AlignX::LEFT,   AlignY::BOTTOM, items...); break;
    case Anchor::WEST:      align(AlignX::LEFT,   AlignY::CENTER, items...); break;
    case Anchor::EAST:      align(AlignX::RIGHT,  AlignY::CENTER, items...); break;
    case Anchor::CENTER:    align(AlignX::CENTER, AlignY::CENTER, items...); break;
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

