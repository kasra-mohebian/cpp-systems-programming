#pragma once
#include <atomic>
#include <cstdint>
#include <memory>
#include <mutex>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>
namespace multimap {
// Fresh public interfaces around the sequential, mutex and CAS insertion concepts.
// Values are immutable after publication. No erase, resize or concurrent destruction.
struct Entry {
    std::int64_t key;
    std::string value;
    Entry *next;
};
inline std::size_t bucket(std::int64_t key, std::size_t count) {
    return static_cast<std::uint64_t>(key) % count;
}
inline std::size_t checkedCount(std::size_t count) {
    if (!count)
        throw std::invalid_argument("bucket count must be positive");
    return count;
}
class SequentialMultiMap {
    std::vector<Entry *> buckets_;

  public:
    explicit SequentialMultiMap(std::size_t count = 64) : buckets_(checkedCount(count), nullptr) {}
    SequentialMultiMap(const SequentialMultiMap &) = delete;
    SequentialMultiMap &operator=(const SequentialMultiMap &) = delete;
    ~SequentialMultiMap() {
        for (auto head : buckets_)
            while (head) {
                auto next = head->next;
                delete head;
                head = next;
            }
    }
    void insert(std::int64_t key, std::string value) {
        auto &head = buckets_[bucket(key, buckets_.size())];
        head = new Entry{key, std::move(value), head};
    }
    std::vector<std::string> values(std::int64_t key) const {
        std::vector<std::string> result;
        for (auto n = buckets_[bucket(key, buckets_.size())]; n; n = n->next)
            if (n->key == key)
                result.push_back(n->value);
        return result;
    }
};
class MutexMultiMap {
    SequentialMultiMap map_;
    mutable std::mutex mutex_;

  public:
    explicit MutexMultiMap(std::size_t count = 64) : map_(count) {}
    void insert(std::int64_t key, std::string value) {
        std::lock_guard lock(mutex_);
        map_.insert(key, std::move(value));
    }
    std::vector<std::string> values(std::int64_t key) const {
        std::lock_guard lock(mutex_);
        return map_.values(key);
    }
};
class AtomicMultiMap {
    std::vector<std::atomic<Entry *>> buckets_;

  public:
    explicit AtomicMultiMap(std::size_t count = 64) : buckets_(checkedCount(count)) {
        for (auto &head : buckets_)
            head.store(nullptr, std::memory_order_relaxed);
    }
    AtomicMultiMap(const AtomicMultiMap &) = delete;
    AtomicMultiMap &operator=(const AtomicMultiMap &) = delete;
    ~AtomicMultiMap() {
        // Caller must join all readers and writers before this object is destroyed.
        for (auto &head : buckets_) {
            auto n = head.load(std::memory_order_relaxed);
            while (n) {
                auto next = n->next;
                delete n;
                n = next;
            }
        }
    }
    void insert(std::int64_t key, std::string value) {
        auto &head = buckets_[bucket(key, buckets_.size())];
        auto node = std::make_unique<Entry>(
            Entry{key, std::move(value), head.load(std::memory_order_relaxed)});
        // node->next remains private until successful release publication.
        while (!head.compare_exchange_weak(node->next, node.get(), std::memory_order_release,
                                           std::memory_order_relaxed)) {
        }
        node.release();
    }
    std::vector<std::string> values(std::int64_t key) const {
        std::vector<std::string> result;
        // Acquire observes the immutable list published by successful insertions.
        auto n = buckets_[bucket(key, buckets_.size())].load(std::memory_order_acquire);
        for (; n; n = n->next)
            if (n->key == key)
                result.push_back(n->value);
        return result;
    }
    bool bucketHeadsAreLockFree() const noexcept { return buckets_.front().is_lock_free(); }
};
} // namespace multimap
