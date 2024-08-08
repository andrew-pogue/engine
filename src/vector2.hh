#pragma once
#include <ostream>

using Vector2d = Vector2<double>;
using Vector2f = Vector2<float>;
using Vector2i = Vector2<int>;
using Vector2u = Vector2<unsigned>;

template <typename T>
struct Vector2 {

    T x, y;

    void operator=(const Vector2<T> &coord) { x = coord.x; y = coord.y; }

    void operator+=(const Vector2<T> &coord) { x += coord.x; y += coord.y; }
    void operator-=(const Vector2<T> &coord) { x -= coord.x; y -= coord.y; }

    void operator*=(T value) { x *= value; y *= value; }
    void operator/=(T value) { x /= value; y /= value; }

    Vector2<T> &&operator+(const Vector2<T> &coord) const { return { x + coord.x, y + coord.y }; }
    Vector2<T> &&operator-(const Vector2<T> &coord) const { return { x - coord.x, y - coord.y }; }

    Vector2<T> &&operator/(T value) const { return { x / value, y / value }; }
    Vector2<T> &&operator*(T value) const { return { x * value, y * value }; }

    std::ostream& operator<<(std::ostream& out) const {
        return out << '{' << coord.x << ',' << coord.y << '}';
    }

};

