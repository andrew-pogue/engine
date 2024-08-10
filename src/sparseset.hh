#pragma once
#include <algorithm>
#include <map>
#include <vector>
#include <iostream>

// https://www.gamedeveloper.com/programming/value-structures-part-1-bulk-value 

template <typename KEY, typename VALUE>
class SparseSet {
public:

    bool has(KEY key) const { return index_.contains(key); }

    VALUE *get(KEY key) {
        auto it = index_.find(key);
        if (it != index_.end())
            return data_.at(it->second).second;
        return nullptr;
    }

    const VALUE *get(KEY key) const {
        auto it = index_.find(key);
        if (it != index_.cend())
            return data_.at(it->second).second;
        return nullptr;
    }

    template <typename... Args>
    void set(KEY key, Args... args) {
        auto [it, inserted] = index_.insert(std::pair{key, data_.size()});
        if (inserted && it != index_.end()) {
            data_.emplace_back(key, VALUE{args...});
        } else if (it != index_.end()) {
            data_.at(it->second).second = VALUE{args...};
        }
    }
    
    void remove(KEY key) {
        auto it = index_.find(key);
        if (it == index_.end()) return;
        data_.at(it->second).first = data_.back().first;
        std::swap(data_.at(it->second).second, data_.back().second);
        index_[data_.at(it->second).first] = it->second;
        data_.pop_back();
    }

    size_t size() { return data_.size(); }

    auto begin() { return data_.begin(); }
    auto end() { return data_.end(); }
    auto cbegin() const { return data_.cbegin(); }
    auto cend() const { return data_.cend(); }

private:

    std::map<KEY, size_t> index_;
    std::vector<std::pair<KEY, VALUE>> data_;

};

