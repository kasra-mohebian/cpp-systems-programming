#include "cppstudy/SortPointers.hpp"
#include <cassert>
#include <vector>

using namespace std;

namespace sort_pointers {

static void swapPointers(const unsigned **a, const unsigned **b) {
    const unsigned *tmp = *a;
    *a = *b;
    *b = tmp;
}

static const unsigned **partitionRange(const unsigned **begin, const unsigned **end) {
    const unsigned *pivotValue = *begin;
    const unsigned **leftPtr = begin + 1;
    const unsigned **rightPtr = end - 1;

    while (true) {
        while (leftPtr <= rightPtr && **leftPtr <= *pivotValue) {
            ++leftPtr;
        }
        while (leftPtr <= rightPtr && **rightPtr > *pivotValue) {
            --rightPtr;
        }
        if (leftPtr >= rightPtr) {
            break;
        }
        swapPointers(leftPtr, rightPtr);
    }

    swapPointers(begin, rightPtr);
    return rightPtr;
}

void quicksort(const unsigned **begin, const unsigned **end) {
    if (begin == end)
        return;
    assert(begin <= end);
    // Recurse only into the smaller partition to bound call-stack depth.
    while (end - begin > 1) {
        const unsigned **pivotPos = partitionRange(begin, end);
        if (pivotPos - begin < end - (pivotPos + 1)) {
            quicksort(begin, pivotPos);
            begin = pivotPos + 1;
        } else {
            quicksort(pivotPos + 1, end);
            end = pivotPos;
        }
    }
}

static void mergeRange(const unsigned **begin, const unsigned **mid, const unsigned **end) {
    vector<const unsigned *> buffer;
    buffer.reserve(end - begin);

    const unsigned **leftPtr = begin;
    const unsigned **rightPtr = mid;

    while (leftPtr < mid && rightPtr < end) {
        if (**leftPtr <= **rightPtr) {
            buffer.push_back(*leftPtr);
            ++leftPtr;
        } else {
            buffer.push_back(*rightPtr);
            ++rightPtr;
        }
    }

    while (leftPtr < mid) {
        buffer.push_back(*leftPtr);
        ++leftPtr;
    }

    while (rightPtr < end) {
        buffer.push_back(*rightPtr);
        ++rightPtr;
    }

    for (size_t i = 0; i < buffer.size(); ++i) {
        begin[i] = buffer[i];
    }
}

void mergesort(const unsigned **begin, const unsigned **end) {
    if (begin == end)
        return;
    assert(begin <= end);

    if (end - begin <= 1) {
        return;
    }

    const unsigned **mid = begin + (end - begin) / 2;
    mergesort(begin, mid);
    mergesort(mid, end);
    mergeRange(begin, mid, end);
}

} // namespace sort_pointers
