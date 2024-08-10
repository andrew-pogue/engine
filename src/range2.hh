#pragma once

#include "vector2.hh"

template <typename T>
struct Range2 {

    class Iterator {
    private:
    
        Vector2<T> from, to, current;
    
    public:
        
        Iterator(Vector2<T> from, Vector2<T> to)
            : from(from), to(to), current(from)
        {}

        Iterator(Vector2<T> from, Vector2<T> to, Vector2<T> current)
            : from(from), to(to), current(current)
        {}
    
        Iterator& operator++() {
            if (to >= from) {
                if (current.x < to.x) current.x++;
                else if (current.y < to.y) current.y++;
            } else {
                if (current.x < to.x) current.x--;
                else if (current.y < to.y) current.y--;
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
        
        const Vector2<T> &operator*() const { return current; }
    
    }; // Iterator

    const Vector2<T> min, max;

    bool contains(const Vector2<T> &vec) const {
        return vec.x >= min.x && vec.y >= min.y
            && vec.x <= max.x && vec.y <= max.y;
    }

    Iterator begin() { return Iterator(min, max, min); }
    Iterator end() { return Iterator(min, max, max); }
    const Iterator cbegin() const { return Iterator(min, max, min); }
    const Iterator cend() const { return Iterator(min, max, max); }

};

using Range2d = Range2<double>;
using Range2f = Range2<float>;
using Range2i = Range2<int>;
using Range2u = Range2<unsigned>;

