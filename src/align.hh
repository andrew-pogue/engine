#pragma once

struct Align {
    enum Horizontal : int {
        NONE = 0b00,
        TOP = 0b01,
        BOTTOM = 0b10,
        CENTER = 0b11,
    };

    Horizontal x : 3 = Horizontal::NONE;

    enum Vertical : int {
        NONE = 0b00,
        LEFT = 0b01,
        RIGHT = 0b10,
        CENTER = 0b11,
    };

    Vertical y : 3 = Vertical::NONE;
};

