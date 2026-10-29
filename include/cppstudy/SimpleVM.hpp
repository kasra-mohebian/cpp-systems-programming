#pragma once
#include <cstdint>
#include <iosfwd>
namespace simplevm {
std::int32_t runVM(std::istream &program, std::ostream &output);
void fibonacciProgram(unsigned n, std::ostream &output);
} // namespace simplevm
