#include "cppstudy/BitSet.hpp"
#include <limits>
#include <stdexcept>
namespace bitset {

BitSet::BitSet() : bytes_(), size_(0) {}

BitSet::BitSet(size_type count) : bytes_(count / 8 + (count % 8 != 0), 0), size_(count) {}

BitSet::size_type BitSet::size() const noexcept { return size_; }

std::uint8_t *BitSet::byte_ptr(size_type index) { return &bytes_[index / 8]; }

const std::uint8_t *BitSet::byte_ptr(size_type index) const { return &bytes_[index / 8]; }

std::uint8_t BitSet::bit_mask(size_type index) {
    return static_cast<std::uint8_t>(1u << (index % 8));
}

BitSet::BitReference BitSet::make_reference(size_type index) {
    return BitReference(byte_ptr(index), bit_mask(index));
}

bool BitSet::get_bit(size_type index) const {
    if (index >= size_)
        throw std::out_of_range("bit index");
    return (*byte_ptr(index) & bit_mask(index)) != 0;
}

// bitReference

BitSet::BitReference::BitReference(std::uint8_t *byte, std::uint8_t mask)
    : byte_(byte), mask_(mask) {}

BitSet::BitReference::operator bool() const { return (*byte_ & mask_) != 0; }

BitSet::BitReference &BitSet::BitReference::operator=(bool value) {
    if (value) {
        *byte_ |= mask_;
    } else {
        *byte_ &= static_cast<std::uint8_t>(~mask_);
    }
    return *this;
}

BitSet::BitReference &BitSet::BitReference::operator=(const BitReference &other) {
    bool value = static_cast<bool>(other);
    return (*this = value);
}
// main BitSet operations

void BitSet::push_back(bool value) {
    if (size_ == std::numeric_limits<size_type>::max())
        throw std::length_error("bitset is full");
    std::size_t byte_index = size_ / 8;

    if (byte_index >= bytes_.size()) {
        bytes_.push_back(0);
    }

    if (value) {
        bytes_[byte_index] |= bit_mask(size_);
    }

    ++size_;
}

bool BitSet::front() const {
    if (!size_)
        throw std::out_of_range("empty bitset");
    return get_bit(0);
}

bool BitSet::back() const {
    if (!size_)
        throw std::out_of_range("empty bitset");
    return get_bit(size_ - 1);
}

BitSet::BitReference BitSet::front() {
    if (!size_)
        throw std::out_of_range("empty bitset");
    return make_reference(0);
}

BitSet::BitReference BitSet::back() {
    if (!size_)
        throw std::out_of_range("empty bitset");
    return make_reference(size_ - 1);
}

bool BitSet::operator[](size_type index) const {
    if (index >= size_)
        throw std::out_of_range("bit index");
    return get_bit(index);
}

BitSet::BitReference BitSet::operator[](size_type index) {
    if (index >= size_)
        throw std::out_of_range("bit index");
    return make_reference(index);
}

std::size_t BitSet::cardinality() const {
    std::size_t count = 0;
    for (std::size_t i = 0; i < size_; ++i) {
        if (get_bit(i)) {
            ++count;
        }
    }
    return count;
}
BitSet::BitIterator BitSet::begin() { return BitIterator(this, 0); }

BitSet::BitIterator BitSet::end() { return BitIterator(this, size_); }

BitSet::ConstBitIterator BitSet::begin() const { return ConstBitIterator(this, 0); }

BitSet::ConstBitIterator BitSet::end() const { return ConstBitIterator(this, size_); }

BitSet::ConstBitIterator BitSet::cbegin() const { return ConstBitIterator(this, 0); }

BitSet::ConstBitIterator BitSet::cend() const { return ConstBitIterator(this, size_); }
// BitIterator

BitSet::BitIterator::BitIterator() : owner_(nullptr), index_(0) {}

BitSet::BitIterator::BitIterator(BitSet *owner, size_type index) : owner_(owner), index_(index) {}

BitSet::BitIterator::reference BitSet::BitIterator::operator*() const { return (*owner_)[index_]; }

BitSet::BitIterator &BitSet::BitIterator::operator++() {
    ++index_;
    return *this;
}

BitSet::BitIterator BitSet::BitIterator::operator++(int) {
    BitIterator temp = *this;
    ++index_;
    return temp;
}

BitSet::BitIterator &BitSet::BitIterator::operator--() {
    --index_;
    return *this;
}

BitSet::BitIterator BitSet::BitIterator::operator--(int) {
    BitIterator temp = *this;
    --index_;
    return temp;
}
// ConstBitIterator

BitSet::ConstBitIterator::ConstBitIterator() : owner_(nullptr), index_(0) {}

BitSet::ConstBitIterator::ConstBitIterator(const BitSet *owner, size_type index)
    : owner_(owner), index_(index) {}

bool BitSet::ConstBitIterator::operator*() const { return (*owner_)[index_]; }

BitSet::ConstBitIterator &BitSet::ConstBitIterator::operator++() {
    ++index_;
    return *this;
}

BitSet::ConstBitIterator BitSet::ConstBitIterator::operator++(int) {
    ConstBitIterator temp = *this;
    ++index_;
    return temp;
}

BitSet::ConstBitIterator &BitSet::ConstBitIterator::operator--() {
    --index_;
    return *this;
}

BitSet::ConstBitIterator BitSet::ConstBitIterator::operator--(int) {
    ConstBitIterator temp = *this;
    --index_;
    return temp;
}

} // namespace bitset
//---------------------------------------------------------------------------
