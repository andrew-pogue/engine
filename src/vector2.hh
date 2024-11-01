#pragma once
#include <ostream>

template <typename T>
struct Vector2 {

    T x, y;

    void operator=(const Vector2<T> &coord) { x = coord.x; y = coord.y; }

    void operator+=(const Vector2<T> &coord) { x += coord.x; y += coord.y; }
    void operator-=(const Vector2<T> &coord) { x -= coord.x; y -= coord.y; }

    void operator*=(T value) { x *= value; y *= value; }
    void operator/=(T value) { x /= value; y /= value; }

    Vector2<T> operator+(const Vector2<T> &coord) const { return { x + coord.x, y + coord.y }; }
    Vector2<T> operator-(const Vector2<T> &coord) const { return { x - coord.x, y - coord.y }; }

    Vector2<T> operator/(T value) const { return { x / value, y / value }; }
    Vector2<T> operator*(T value) const { return { x * value, y * value }; }

    std::ostream& operator<<(std::ostream& out) const { return out << '{' << x << ',' << y << '}'; }

    bool operator==(const Vector2<T> &other) const { return x == other.x && y == other.y; }
    bool operator!=(const Vector2<T> &other) const { return x != other.x || y != other.y; }

};

using Vec2d = Vector2<double>;
using Vec2f = Vector2<float>;
using Vec2i = Vector2<int>;
using Vec2u = Vector2<unsigned>;

