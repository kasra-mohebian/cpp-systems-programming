#pragma once
#include <ostream>
#include <vector>
namespace binary_heap {
void insert(std::vector<unsigned> &heap, unsigned value);
unsigned extract(std::vector<unsigned> &heap);
void printDot(std::ostream &out, const std::vector<unsigned> &heap);
} // namespace binary_heap
