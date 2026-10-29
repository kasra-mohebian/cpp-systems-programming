#pragma once
namespace sort_pointers {
// Half-open range in one array, containing valid pointers to unsigned values.
void quicksort(const unsigned **begin, const unsigned **end);
void mergesort(const unsigned **begin, const unsigned **end);
} // namespace sort_pointers
