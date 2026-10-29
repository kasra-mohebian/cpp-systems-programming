#pragma once
#include <cstddef>
#include <limits>
#include <stdexcept>
#include <vector>
namespace union_find {
// Independent public interface; union-by-rank and compression follow my implementation.
class UnionFind {
    struct Entry {
        unsigned parent;
        unsigned rank = 0;
    };
    mutable std::vector<Entry> entries;

  public:
    explicit UnionFind(unsigned size) {
        entries.reserve(size);
        for (unsigned i = 0; i < size; ++i)
            entries.push_back({i});
    }
    unsigned add() {
        if (entries.size() >= std::numeric_limits<unsigned>::max())
            throw std::length_error("too many sets");
        auto index = static_cast<unsigned>(entries.size());
        entries.push_back({index});
        return index;
    }
    unsigned find(unsigned entry) const {
        if (entry >= entries.size())
            throw std::out_of_range("set index");
        unsigned root = entry;
        while (entries[root].parent != root)
            root = entries[root].parent;
        while (entry != root) {
            unsigned parent = entries[entry].parent;
            entries[entry].parent = root;
            entry = parent;
        }
        return root;
    }
    unsigned unionSets(unsigned a, unsigned b) {
        auto ra = find(a), rb = find(b);
        if (ra == rb)
            return ra;
        if (entries[ra].rank < entries[rb].rank) {
            entries[ra].parent = rb;
            return rb;
        }
        if (entries[rb].rank < entries[ra].rank) {
            entries[rb].parent = ra;
            return ra;
        }
        ++entries[ra].rank;
        entries[rb].parent = ra;
        return ra;
    }
    bool disjoint(unsigned a, unsigned b) const { return find(a) != find(b); }
};
} // namespace union_find
