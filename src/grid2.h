#pragma once
#include <cassert>
#include <iostream>
#include "vector2.hh"

template <typename T>
class Grid2 {
private:

    const size_t size;
    T *data;

    size_t index(int x, int y) const {
        assert(x + (y * width) < size() && "[Grid2] index out of range");
        return x + (y * width);
    }

public:

    const unsigned width, height;

    Grid2(unsigned w, unsigned h)
        : width(w), height(h)
        , data(new T[size()]())
    {}

    Grid2(const Grid2 &other)
        : width(other.width), height(other.width)
        , data(new T[size()]())
    {
        for (size_t i = 0; i < size(); i++) data[i] = other.data[i];
    }

    Grid2(Grid2 &&other) 
        : width(other.width), height(other.width)
        , data(other.data)
    {
        assert(data_ && "[Grid2] data is null?");
        other.data = nullptr;
    }

    ~Grid2() { delete[] data; }

    void operator=(Grid2 &&other) {
        width = other.width; height = other.height;
        delete[] data; data = other.data; other.data = nullptr;
    }

    void operator=(const Grid2 &other) {
        width = other.width; height = other.height;
        delete[] data; data = new T[size()]();
        for (size_t i = 0; i < size(); i++) data[i] = other.data[i];
    }

    unsigned size() const { return width * height; }

    bool has(int x, int y) const { return (x < width) && (y < height); }
    bool has(Vector2 coord) const { return has(coord.x, coord.y); }

    T &operator[](int x, int y) {
        assert(has(x,y) && "[Grid2] invalid coordinates passed");
        return data[index(x,y)];
    }
    const T &operator[](int x, int y) const {
        assert(has(x,y) && "[Grid2] invalid coordinates passed");
        return data[index(x,y)];
    }

          T &operator[](Vector2 coord)       { return (*this)[coord.x, coord.y]; }
    const T &operator[](Vector2 coord) const { return (*this)[coord.x, coord.y]; }

          T *get(int x, int y)       { return has(x,y) ? data[index(x,y)] : nullptr; }
    const T *get(int x, int y) const { return has(x,y) ? data[index(x,y)] : nullptr; }

          T *get(Vector2 coord)       { return has(coord) ? (*this)[coord.x, coord.y] : nullptr; }
    const T *get(Vector2 coord) const { return has(coord) ? (*this)[coord.x, coord.y] : nullptr; }

};

