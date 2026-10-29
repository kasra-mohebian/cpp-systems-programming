#include "cppstudy/ChainingHashTable.hpp"

#include <cstdint>
#include <utility>

namespace hashtable {
ChainingHashTable::ChainingHashTable(ChainingHashTable &&other) noexcept
    : buckets(std::move(other.buckets)), entryCount(std::exchange(other.entryCount, 0)) {}
ChainingHashTable &ChainingHashTable::operator=(ChainingHashTable &&other) noexcept {
    if (this != &other) {
        ChainingHashTable old(std::move(*this));
        buckets = std::move(other.buckets);
        entryCount = std::exchange(other.entryCount, 0);
    }
    return *this;
}

std::size_t ChainingHashTable::bucketFor(int64_t key, std::size_t bucketCount) {
    if (bucketCount == 0)
        return 0;
    const std::uint64_t u = static_cast<std::uint64_t>(key);
    return static_cast<std::size_t>(u % bucketCount);
}

ChainingHashTable::~ChainingHashTable() {
    for (auto &head : buckets) {
        std::unique_ptr<Node> cur = std::move(head);
        while (cur) {
            std::unique_ptr<Node> next = std::move(cur->next);
            cur->next.reset();
            cur = std::move(next);
        }
        head.reset();
    }
    buckets.clear();
    entryCount = 0;
}

std::size_t ChainingHashTable::size() const { return entryCount; }

ChainingHashTable::Node *ChainingHashTable::findNode(int64_t key) const {
    std::size_t dummy = 0;
    return findNode(key, dummy);
}

ChainingHashTable::Node *ChainingHashTable::findNode(int64_t key,
                                                     std::size_t &outBucketIndex) const {
    if (buckets.empty())
        return nullptr;

    const std::size_t idx = bucketFor(key, buckets.size());
    outBucketIndex = idx;

    Node *cur = buckets[idx].get();
    while (cur) {
        if (cur->entry.key == key)
            return cur;
        cur = cur->next.get();
    }
    return nullptr;
}

bool ChainingHashTable::contains(int64_t key) const { return findNode(key) != nullptr; }

void ChainingHashTable::ensureBucketsForInsert() {
    if (buckets.empty()) {
        buckets.resize(16);
    }
}

void ChainingHashTable::rehash(std::size_t newBucketCount) {
    std::vector<std::unique_ptr<Node>> newBuckets(newBucketCount);

    for (auto &head : buckets) {
        std::unique_ptr<Node> cur = std::move(head);
        while (cur) {
            std::unique_ptr<Node> next = std::move(cur->next);

            const std::size_t idx = bucketFor(cur->entry.key, newBucketCount);
            cur->next = std::move(newBuckets[idx]);
            newBuckets[idx] = std::move(cur);

            cur = std::move(next);
        }
        head.reset();
    }

    buckets = std::move(newBuckets);
}

BytePayload &ChainingHashTable::insert(int64_t key, BytePayload &&value) {
    ensureBucketsForInsert();

    std::size_t idx = 0;
    Node *existing = findNode(key, idx);
    if (existing) {
        existing->entry.value = std::move(value);
        return existing->entry.value;
    }

    idx = bucketFor(key, buckets.size());
    buckets[idx] = std::make_unique<Node>(key, std::move(value), std::move(buckets[idx]));
    ++entryCount;

    if (static_cast<double>(entryCount) / static_cast<double>(buckets.size()) > 0.5) {
        rehash(buckets.size() * 2);
    }

    std::size_t idx2 = 0;
    Node *again = findNode(key, idx2);
    return again->entry.value;
}

BytePayload &ChainingHashTable::insert(Entry entry) {
    return insert(entry.key, std::move(entry.value));
}

BytePayload &ChainingHashTable::operator[](int64_t key) {
    ensureBucketsForInsert();

    std::size_t idx = 0;
    Node *existing = findNode(key, idx);
    if (existing)
        return existing->entry.value;

    BytePayload v;
    return insert(key, std::move(v));
}

void ChainingHashTable::erase(int64_t key) {
    if (buckets.empty())
        return;

    const std::size_t idx = bucketFor(key, buckets.size());
    std::unique_ptr<Node> *link = &buckets[idx];

    while (*link) {
        if ((*link)->entry.key == key) {
            auto removed = std::move(*link);
            *link = std::move(removed->next);
            --entryCount;
            return;
        }
        link = &((*link)->next);
    }
}

ChainingHashTable::iterator ChainingHashTable::begin() {
    if (entryCount == 0 || buckets.empty())
        return end();

    for (std::size_t i = 0; i < buckets.size(); ++i) {
        if (buckets[i])
            return iterator(this, i, buckets[i].get());
    }
    return end();
}

ChainingHashTable::iterator ChainingHashTable::end() { return iterator(nullptr, 0, nullptr); }

ChainingHashTable::iterator ChainingHashTable::find(int64_t key) {
    std::size_t idx = 0;
    Node *n = findNode(key, idx);
    if (!n)
        return end();
    return iterator(this, idx, n);
}

ChainingHashTable::iterator::reference ChainingHashTable::iterator::operator*() const {
    return node->entry;
}

ChainingHashTable::iterator::pointer ChainingHashTable::iterator::operator->() const {
    return &node->entry;
}

ChainingHashTable::iterator &ChainingHashTable::iterator::operator++() {
    if (!node || !table) {
        table = nullptr;
        node = nullptr;
        bucketIndex = 0;
        return *this;
    }

    if (node->next) {
        node = node->next.get();
        return *this;
    }

    std::size_t i = bucketIndex + 1;
    while (i < table->buckets.size() && !table->buckets[i]) {
        ++i;
    }

    if (i >= table->buckets.size()) {
        table = nullptr;
        node = nullptr;
        bucketIndex = 0;
        return *this;
    }

    bucketIndex = i;
    node = table->buckets[i].get();
    return *this;
}

ChainingHashTable::iterator ChainingHashTable::iterator::operator++(int) {
    iterator tmp = *this;
    ++(*this);
    return tmp;
}

bool operator==(const ChainingHashTable::iterator &a, const ChainingHashTable::iterator &b) {
    if (!a.node && !b.node)
        return true;
    return a.table == b.table && a.bucketIndex == b.bucketIndex && a.node == b.node;
}

bool operator!=(const ChainingHashTable::iterator &a, const ChainingHashTable::iterator &b) {
    return !(a == b);
}

} // namespace hashtable
