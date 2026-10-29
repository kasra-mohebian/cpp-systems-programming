#include "cppstudy/BinaryHeap.hpp"
#include "cppstudy/MultiMaps.hpp"
#include "cppstudy/Rational.hpp"
#include "cppstudy/SimpleVM.hpp"
#include "cppstudy/StringMatching.hpp"
#include "cppstudy/Varint.hpp"
#include <iostream>
#include <sstream>
#include <thread>
int main() {
    std::vector<unsigned> heap;
    for (unsigned n : {3, 9, 1, 7})
        binary_heap::insert(heap, n);
    std::cout << "Heap, largest first:";
    while (!heap.empty())
        std::cout << ' ' << binary_heap::extract(heap);
    auto sum = arithmetic::Rational(1, 3) + arithmetic::Rational(1, 6);
    std::cout << "\n1/3 + 1/6 = " << sum.num() << '/' << sum.den();
    std::cout << "\nKMP match offset = "
              << string_matching::matchKMP("systems programming", "program");
    auto bytes = varint::compress({0, 127, 128, 16384});
    std::cout << "\nVarint: 4 integers -> " << bytes.size() << " bytes";
    std::stringstream program;
    std::ostringstream diagnostics;
    simplevm::fibonacciProgram(10, program);
    std::cout << "\nVM Fibonacci(10) = " << simplevm::runVM(program, diagnostics);
    multimap::AtomicMultiMap map;
    std::thread a([&] { map.insert(7, "first"); }), b([&] { map.insert(7, "second"); });
    a.join();
    b.join();
    std::cout << "\nConcurrent inserts for key 7 = " << map.values(7).size() << '\n';
}
