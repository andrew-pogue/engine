#pragma once
#include <cassert>
#include <iostream>
#include "grid3.hh"

template <typename T>
class Grid3 {
private:

    const size_t size;
    T *data;

    size_t index(int x, int y, int z) const {
        assert(x + (y * width) + (z * width * height) < size() && "[Grid3] index out of range");
        return x + (y * width) + (z * width * height);
    }

public:

    const unsigned width, height, depth;

    Grid3(unsigned w, unsigned h, unsigned d)
        : width(w), height(h), depth(d)
        , data(new T[size()]())
    {}

    Grid3(const Grid3 &other)
        : width(other.width), height(other.width), depth(other.depth)
        , data(new T[size()]())
    {
        for (size_t i = 0; i < size(); i++) data[i] = other.data[i];
    }

    Grid3(Grid3 &&other) 
        : width(other.width), height(other.width), depth(other.depth)
        , data(other.data)
    {
        assert(data_ && "[Grid3] data is null?");
        other.data = nullptr;
    }

    ~Grid3() { delete[] data; }

    void operator=(Grid3 &&other) {
        width = other.width; height = other.height; depth = other.depth;
        delete[] data; data = other.data; other.data = nullptr;
    }

    void operator=(const Grid3 &other) {
        width = other.width; height = other.height; depth = other.depth;
        delete[] data; data = new T[size()]();
        for (size_t i = 0; i < size(); i++) data[i] = other.data[i];
    }

    unsigned size() const { return width * height * depth; }

    bool has(int x, int y, int z) const { return (x < width) && (y < height) && (z < depth); }
    bool has(Vector3 coord) const { return has(coord.x, coord.y, coord.z); }

    T &operator[](int x, int y, int z) {
        assert(has(x, y, z) && "[Grid3] invalid coordinates passed");
        return data[index(x,y,z)];
    }
    const T &operator[](int x, int y, int z) const {
        assert(has(x, y, z) && "[Grid3] invalid coordinates passed");
        return data[index(x,y,z)];
    }

          T &operator[](Vector3 coord)       { return (*this)[coord.x, coord.y, coord.z]; }
    const T &operator[](Vector3 coord) const { return (*this)[coord.x, coord.y, coord.z]; }

          T *get(int x, int y, int z)       { return has(x,y,z) ? data[index(x,y,z)] : nullptr; }
    const T *get(int x, int y, int z) const { return has(x,y,z) ? data[index(x,y,z)] : nullptr; }

          T *get(Vector3 coord)       { return has(coord) ? (*this)[coord.x, coord.y, coord.z] : nullptr; }
    const T *get(Vector3 coord) const { return has(coord) ? (*this)[coord.x, coord.y, coord.z] : nullptr; }

};

