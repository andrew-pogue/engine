#pragma once

class ScopedClippingRectangle {
public:

    ScopedClippingRectangle(float x, float y, float w, float h) {
        al_get_clipping_rectangle(&px, &py, &pw, &ph); 
        al_set_clipping_rectangle(x, y, w, h);
    }

    ~ScopedClippingRectangle() {
        al_set_clipping_rectangle(px, py, pw, ph);
    }

private:

    float px, py, pw, ph;

};

