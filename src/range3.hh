#pragma once

#include "vector3.hh"

template <typename T>
struct Range3 {

    class Iterator {
    private:
    
        Vector3<T> from, to, current;
    
    public:

        Iterator(Vector3<T> from, Vector3<T> to)
            : from(from), to(to), current(from)
        {}

        Iterator(Vector3<T> from, Vector3<T> to, Vector3<T> current)
            : from(from), to(to), current(current)
        {}
    
        Iterator& operator++() {
            if (to >= from) {
                if (current.x < to.x) current.x++;
                else if (current.y < to.y) current.y++;
                else if (current.z < to.z) current.z++;
            } else {
                if (current.x < to.x) current.x--;
                else if (current.y < to.y) current.y--;
                else if (current.z < to.z) current.z--;
            }
            return current;
        }

        Iterator operator++(int) {
            Iterator postval = *this;
            ++(*this);
            return postval;
        }
        
        bool operator==(const Iterator &other) const { return current == other.current; }
        bool operator!=(const Iterator &other) const { return current != other.current; }
        
        const Vector3<T> &operator*() const { return current; }
    
    }; // Iterator

    const Vector3<T> min, max;

    bool contains(const Vector3<T> &vec) const {
        return vec.x >= min.x && vec.y >= min.y && vec.z >= min.z
            && vec.x <= max.x && vec.y <= max.y && vec.z <= max.z;
    }

    Iterator begin() { return Iterator(min, max, min); }
    Iterator end() { return Iterator(min, max, max); }
    const Iterator cbegin() const { return Iterator(min, max, min); }
    const Iterator cend() const { return Iterator(min, max, max); }

};

using Range3d = Range3<double>;
using Range3f = Range3<float>;
using Range3i = Range3<int>;
using Range3u = Range3<unsigned>;

