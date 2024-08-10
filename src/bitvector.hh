#pragma once
#include <cstdint>
#include <vector>

class BitVector {
private:

    std::vector<uint64_t> bytes_;
    size_t count_; // The number of bits set to true

public:

    BitVector(size_t size=0)
        : bytes_(size/64u+1u, 0u)
    {
    }

    BitVector(const BitVector &other)
        : bytes_(other.bytes_)
        , count_(other.count_)
    {
    }

    BitVector(BitVector &&other)
        : bytes_(std::move(other.bytes_))
        , count_(other.count_)
    {
    }

    // Returns the value of the i'th bit.
    bool test(size_t i) const {
        if (i/64 > bytes_.size())
            return false;
        return bytes_[i/64] & 1<<i%64;
    }

    // Sets the i'th bit to true.
    void set(size_t i) {
        if (i/64 > bytes_.size())
            bytes_.resize(i/64, 0);
        bytes_[i/64] |= 1<<i%64;
        count_++;
    }

    // Sets the i'th bit to false.
    void unset(size_t i) {
        if (i/64 > bytes_.size())
            return;
        bytes_[i/64] &= ~1<<i%64;
        count_--;
    }

    // Returns the number of bits set to true.
    size_t count() const {
        return count_;
    }

    void operator=(BitVector &&other) {
        bytes_ = std::move(other.bytes_);
        count_ = other.count_;
    }

    void operator=(const BitVector &other) {
        bytes_ = other.bytes_;
        count_ = other.count_;
    }

    bool operator==(const BitVector &other) const {
        if (count_ != other.count_)
            return false;

        const auto *greater = &bytes_,
            *lesser = &other.bytes_;
        if (greater->size() < lesser->size()) {
            auto temp = greater;
            greater = lesser;
            lesser = temp;
        }

        for (size_t i=lesser->size(); i < greater->size(); i++)
            if (greater->at(i)) return false;

        for (size_t i=0; i<lesser->size(); i++)
            if (lesser->at(i) != greater->at(i)) return false;

        return true;
    }

    // Returns true iff every bit set in other is also set in this.
    bool has(const BitVector &other) const {
        if (count_ != other.count_)
            return false;

        size_t end = other.bytes_.size();
        if (end > bytes_.size()) {
            for (size_t i=bytes_.size(); i < end; i++)
                if (other.bytes_.at(i)) return false;
            end = bytes_.size();
        }

        for (size_t i=0; i < end; i++)
            if (bytes_.at(i) & other.bytes_.at(i) != other.bytes_.at(i))
                return false;

        return true;
    }

    void shrink_to_fit() {
        while(bytes_.back()==0)
            bytes_.pop_back();
        bytes_.shrink_to_fit();
    }

};