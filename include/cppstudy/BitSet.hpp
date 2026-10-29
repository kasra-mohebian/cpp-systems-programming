#pragma once
#include <cstddef>
#include <cstdint>
#include <iterator>
#include <utility>
#include <vector>
namespace bitset {
// packed container for bool values stored in bytes.
class BitSet {
  public:
    class BitReference {
      public:
        BitReference(const BitReference &) = default;
        BitReference &operator=(bool value);
        BitReference &operator=(const BitReference &other);
        operator bool() const;

      private:
        friend class BitSet;

        BitReference(std::uint8_t *byte, std::uint8_t mask);

        std::uint8_t *byte_{};
        std::uint8_t mask_{};
    };

    class BitIterator;
    class ConstBitIterator;

    using size_type = std::size_t;

    BitSet();
    BitSet(const BitSet &) = default;
    BitSet &operator=(const BitSet &) = default;
    BitSet(BitSet &&other) noexcept
        : bytes_(std::move(other.bytes_)), size_(std::exchange(other.size_, 0)) {
        other.bytes_.clear();
    }
    BitSet &operator=(BitSet &&other) noexcept {
        if (this != &other) {
            bytes_ = std::move(other.bytes_);
            size_ = std::exchange(other.size_, 0);
            other.bytes_.clear();
        }
        return *this;
    }
    explicit BitSet(size_type count);

    size_type size() const noexcept;
    std::size_t cardinality() const;

    void push_back(bool value);

    bool front() const;
    bool back() const;
    BitReference front();
    BitReference back();

    bool operator[](size_type index) const;
    BitReference operator[](size_type index);

    BitIterator begin();
    BitIterator end();
    ConstBitIterator begin() const;
    ConstBitIterator end() const;
    ConstBitIterator cbegin() const;
    ConstBitIterator cend() const;

    class BitIterator {
      public:
        using difference_type = std::ptrdiff_t;
        using value_type = bool;
        using reference = BitReference;
        using iterator_category = std::bidirectional_iterator_tag;

        BitIterator();

      private:
        friend class BitSet;

        BitIterator(BitSet *owner, size_type index);

        BitSet *owner_{};
        size_type index_{};

      public:
        reference operator*() const;
        BitIterator &operator++();
        BitIterator operator++(int);
        BitIterator &operator--();
        BitIterator operator--(int);

        friend bool operator==(const BitIterator &a, const BitIterator &b) {
            return a.owner_ == b.owner_ && a.index_ == b.index_;
        }

        friend bool operator!=(const BitIterator &a, const BitIterator &b) { return !(a == b); }
    };

    class ConstBitIterator {
      public:
        using difference_type = std::ptrdiff_t;
        using value_type = bool;
        using reference = bool;
        using iterator_category = std::bidirectional_iterator_tag;

        ConstBitIterator();

      private:
        friend class BitSet;

        ConstBitIterator(const BitSet *owner, size_type index);

        const BitSet *owner_{};
        size_type index_{};

      public:
        bool operator*() const;
        ConstBitIterator &operator++();
        ConstBitIterator operator++(int);
        ConstBitIterator &operator--();
        ConstBitIterator operator--(int);

        friend bool operator==(const ConstBitIterator &a, const ConstBitIterator &b) {
            return a.owner_ == b.owner_ && a.index_ == b.index_;
        }

        friend bool operator!=(const ConstBitIterator &a, const ConstBitIterator &b) {
            return !(a == b);
        }
    };

  private:
    std::vector<std::uint8_t> bytes_;
    size_type size_{0};

    BitReference make_reference(size_type index);
    bool get_bit(size_type index) const;

    std::uint8_t *byte_ptr(size_type index);
    const std::uint8_t *byte_ptr(size_type index) const;
    static std::uint8_t bit_mask(size_type index);
};
} // namespace bitset

//---------------------------------------------------------------------------
