#pragma once
#include <ostream>

template <typename T>
struct Vector3 {

    T x, y, z;

    void operator=(const Vector3<T> &other) { x = other.x; y = other.y; z = other.z; }

    void operator+=(const Vector3<T> &other) { x += other.x; y += other.y; z += other.z; }
    void operator-=(const Vector3<T> &other) { x -= other.x; y -= other.y; z -= other.z; }

    void operator*=(T value) { x *= value; y *= value; z *= value; }
    void operator/=(T value) { x /= value; y /= value; z /= value; }

    Vector3<T> operator+(const Vector3<T> &other) const { return { x + other.x, y + other.y, z + other.z }; }
    Vector3<T> operator-(const Vector3<T> &other) const { return { x - other.x, y - other.y, z - other.z }; }

    Vector3<T> operator/(T value) const { return { x / value, y / value, z / value }; }
    Vector3<T> operator*(T value) const { return { x * value, y * value, z * value }; }

    std::ostream& operator<<(std::ostream& out) const { return out << '{' << x << ',' << y << ',' << z << '}'; }

};

using Vec3d = Vector3<double>;
using Vec3f = Vector3<float>;
using Vec3i = Vector3<int>;
using Vec3u = Vector3<unsigned>;

