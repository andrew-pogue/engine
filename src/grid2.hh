#pragma once
#include <cassert>
#include <iostream>
#include "vector2.hh"

template <typename T>
struct Grid2 {

    const unsigned width, height;

    Grid2(unsigned w, unsigned h, const T &fill)
        : width(w), height(h)
        , data_{new T[size()](fill)}
    { }

    Grid2(const Grid2 &other)
        : width(other.width), height(other.width)
        , data_(new T[size()]())
    {
        for (size_t i = 0; i < size(); i++) data_[i] = other.data_[i];
    }

    Grid2(Grid2 &&other) 
        : width(other.width), height(other.width)
        , data_(other.data_)
    {
        assert(data_ && "[Grid2] data is null?");
        other.data_ = nullptr;
    }

    ~Grid2() { delete[] data_; }

    void operator=(Grid2 &&other) {
        width = other.width; height = other.height;
        delete[] data_; data_ = other.data_; other.data_ = nullptr;
    }

    void operator=(const Grid2 &other) {
        width = other.width; height = other.height;
        delete[] data_; data_ = new T[size()]();
        for (size_t i = 0; i < size(); i++) data_[i] = other.data_[i];
    }

    unsigned size() const { return width * height; }

    bool contains(int x, int y) const { return x >= 0 && x < width && y >= 0 && y < height; }
    bool contains(Vector2<int> coord) const { return contains(coord.x, coord.y); }

    const T &at(int x, int y) const {
        assert(contains(x,y) && "[Grid2] invalid coordinates passed");
        return data_[index(x,y)];
    }
    T &at(int x, int y) {
        assert(contains(x,y) && "[Grid2] invalid coordinates passed");
        return data_[index(x,y)];
    }

    const T &at(Vector2<int> coord) const { return at(coord.x, coord.y); }
          T &at(Vector2<int> coord)       { return at(coord.x, coord.y); }

    const T *get(int x, int y) const { return contains(x,y) ? data_[index(x,y)] : nullptr; }
          T *get(int x, int y)       { return contains(x,y) ? data_[index(x,y)] : nullptr; }

    const T *get(Vector2<int> coord) const { return contains(coord) ? at(coord.x, coord.y) : nullptr; }
          T *get(Vector2<int> coord)       { return contains(coord) ? at(coord.x, coord.y) : nullptr; }

    const T *begin() const { return data_; }
          T *begin()       { return data_; }
    const T *end() const { return data_ + size(); }
          T *end()       { return data_ + size(); }

private:

    T *data_;

    size_t index(int x, int y) const {
        assert(x + (y * width) < size() && "[Grid2] index out of range");
        return x + (y * width);
    }

};

