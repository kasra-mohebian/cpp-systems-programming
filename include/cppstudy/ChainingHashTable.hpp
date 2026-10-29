#pragma once
#include "cppstudy/BytePayload.hpp"

#include <cstddef>
#include <cstdint>
#include <iterator>
#include <memory>
#include <utility>
#include <vector>
namespace hashtable {

class ChainingHashTable {
  public:
    class Entry {
      public:
        const int64_t key;
        BytePayload value;

        Entry(int64_t k, BytePayload v) : key(k), value(std::move(v)) {}
    };

  private:
    struct Node {
        Entry entry;
        std::unique_ptr<Node> next;

        Node(int64_t k, BytePayload v, std::unique_ptr<Node> n)
            : entry(k, std::move(v)), next(std::move(n)) {}
    };

  public:
    class iterator {
      public:
        using value_type = Entry;
        using difference_type = std::ptrdiff_t;
        using reference = Entry &;
        using pointer = Entry *;
        using iterator_category = std::forward_iterator_tag;

        iterator() = default;

        reference operator*() const;
        pointer operator->() const;

        iterator &operator++();
        iterator operator++(int);

        friend bool operator==(const iterator &a, const iterator &b);
        friend bool operator!=(const iterator &a, const iterator &b);

      private:
        friend class ChainingHashTable;

        ChainingHashTable *table = nullptr;
        std::size_t bucketIndex = 0;
        Node *node = nullptr;

        iterator(ChainingHashTable *t, std::size_t b, Node *n)
            : table(t), bucketIndex(b), node(n) {}
    };

    ChainingHashTable() = default;
    ChainingHashTable(ChainingHashTable &&other) noexcept;
    ChainingHashTable &operator=(ChainingHashTable &&other) noexcept;

    ChainingHashTable(const ChainingHashTable &) = delete;
    ChainingHashTable &operator=(const ChainingHashTable &) = delete;

    ~ChainingHashTable();

    std::size_t size() const;
    bool contains(int64_t key) const;

    BytePayload &operator[](int64_t key);

    BytePayload &insert(int64_t key, BytePayload &&value);
    BytePayload &insert(Entry entry);

    void erase(int64_t key);

    iterator begin();
    iterator end();
    iterator find(int64_t key);

  private:
    std::vector<std::unique_ptr<Node>> buckets;
    std::size_t entryCount = 0;

    static std::size_t bucketFor(int64_t key, std::size_t bucketCount);
    void ensureBucketsForInsert();
    void rehash(std::size_t newBucketCount);

    Node *findNode(int64_t key) const;
    Node *findNode(int64_t key, std::size_t &outBucketIndex) const;
};

} // namespace hashtable
