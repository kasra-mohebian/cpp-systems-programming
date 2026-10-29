#include "cppstudy/BinaryHeap.hpp"
#include <stdexcept>
#include <utility>

namespace binary_heap {

namespace {
std::size_t parentIndex(std::size_t i) { return (i - 1) / 2; }

std::size_t leftChildIndex(std::size_t i) { return 2 * i + 1; }

std::size_t rightChildIndex(std::size_t i) { return 2 * i + 2; }

void printEdges(std::ostream &out, const std::vector<unsigned> &heap, std::size_t index) {
    std::size_t left = leftChildIndex(index);
    std::size_t right = rightChildIndex(index);

    if (left < heap.size()) {
        out << "\t" << index << " -> " << left << ";\n";
        printEdges(out, heap, left);
    }

    if (right < heap.size()) {
        out << "\t" << index << " -> " << right << ";\n";
        printEdges(out, heap, right);
    }
}
} // namespace

void insert(std::vector<unsigned> &heap, unsigned value) {
    heap.push_back(value);
    std::size_t index = heap.size() - 1;

    while (index > 0) {
        std::size_t parent = parentIndex(index);
        if (heap[parent] >= heap[index]) {
            break;
        }
        std::swap(heap[parent], heap[index]);
        index = parent;
    }
}

unsigned extract(std::vector<unsigned> &heap) {
    if (heap.empty())
        throw std::out_of_range("extract from empty heap");
    unsigned result = heap.front();
    heap[0] = heap.back();
    heap.pop_back();

    std::size_t index = 0;

    while (true) {
        std::size_t left = leftChildIndex(index);
        std::size_t right = rightChildIndex(index);

        if (left >= heap.size()) {
            break;
        }

        std::size_t largest = left;
        if (right < heap.size() && heap[right] > heap[left]) {
            largest = right;
        }

        if (heap[index] >= heap[largest]) {
            break;
        }

        std::swap(heap[index], heap[largest]);
        index = largest;
    }

    return result;
}

void printDot(std::ostream &out, const std::vector<unsigned> &heap) {
    out << "digraph {\n";

    for (std::size_t i = 0; i < heap.size(); ++i) {
        out << "\t" << i << " [label=\"" << heap[i] << "\"];\n";
    }

    if (!heap.empty()) {
        printEdges(out, heap, 0);
    }

    out << "}\n";
}

} // namespace binary_heap
