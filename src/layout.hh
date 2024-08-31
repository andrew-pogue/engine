#pragma once
#include "align.hh"
#include "panel.hh"

void standard_align_layout(Widget &widget, Align align, float width, float height) {
    switch(align.y) {
    case ALIGN_CENTER: widget.y += (height - widget.height) / 2.0f; break;
    case ALIGN_BOTTOM: widget.y += height - widget.height; break;
    }
    switch(align.x) {
    case ALIGN_CENTER: widget.x += (width - widget.width) / 2.0f; break;
    case ALIGN_RIGHT: widget.x += width - widget.width; break;
    }
}

struct Row : Panel {

    Row(float x, float y, float w, float h, Align align)
        : Panel(x, y, w, h)
        , align(align)
    {}

    void layout() override {
        float shift_x = 0.0f;
        for (auto widget : widgets_) {
            standard_align_layout(*widget, align, width, height);
            switch (align.x) {
            case ALIGN_LEFT: widget->x += shift_x; break;
            case ALIGN_CENTER: widget->x -= shift_x / 2.0f; break;
            case ALIGN_RIGHT: widget->x -= shift_x; break;
            }
            shift_x += widget->width;
        }
    }

    Align align;

};

struct Column : Panel {

    Column(float x, float y, float w, float h, Align align)
        : Panel(x, y, w, h)
        , align(align)
    {}

    void layout() override {
        float shift_y = 0.0f;
        for (auto widget : widgets_) {
            standard_align_layout(*widget, width, height);
            switch (align.y) {
            case ALIGN_TOP: widget->y += shift_y; break;
            case ALIGN_CENTER: widget->y -= shift_y / 2.0f; break;
            case ALIGN_BOTTOM: widget->y -= shift_y; break;
            }
            shift_y += widget->height;
        }
    }

    Align align;

};

