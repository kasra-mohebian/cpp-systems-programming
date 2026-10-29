#include "cppstudy/ObjectRepresentation.hpp"
#include <cstdint>
#include <cstring>
#include <iostream>
#include <limits>
#include <vector>
static_assert(sizeof(float) == 4 && sizeof(double) == 8);
static_assert(std::numeric_limits<float>::is_iec559 && std::numeric_limits<double>::is_iec559);

using namespace std;

namespace object_representation {

namespace {
void printBits(std::uint64_t value, unsigned count) {
    for (int i = static_cast<int>(count) - 1; i >= 0; --i) {
        std::uint64_t mask = std::uint64_t(1) << i;
        std::cout << ((value & mask) ? '1' : '0');
    }
}

using Byte = unsigned char;
} // namespace

void printBinary(float value) {
    std::uint32_t bits = 0;
    std::memcpy(&bits, &value, sizeof(float));

    std::uint32_t sign = (bits >> 31) & 0x1u;
    std::uint32_t exponent = (bits >> 23) & 0xFFu;
    std::uint32_t mantissa = bits & ((1u << 23) - 1u);

    std::cout << "sign: " << sign << '\n';

    std::cout << "exponent: ";
    printBits(exponent, 8);
    std::cout << '\n';

    std::cout << "mantissa: ";
    printBits(mantissa, 23);
    std::cout << '\n';
}

void printBinary(double value) {
    std::uint64_t bits = 0;
    std::memcpy(&bits, &value, sizeof(double));

    std::uint64_t sign = (bits >> 63) & 0x1ull;
    std::uint64_t exponent = (bits >> 52) & 0x7FFull;
    std::uint64_t mantissa = bits & ((1ull << 52) - 1ull);

    std::cout << "sign: " << sign << '\n';

    std::cout << "exponent: ";
    printBits(exponent, 11);
    std::cout << '\n';

    std::cout << "mantissa: ";
    printBits(mantissa, 52);
    std::cout << '\n';
}

void push(std::vector<Byte> &stack, float value) {
    const std::size_t n = sizeof(float);
    const Byte *bytes = reinterpret_cast<const Byte *>(&value);

    for (std::size_t i = 0; i < n; ++i) {
        stack.push_back(bytes[i]);
    }
    stack.push_back(static_cast<Byte>(n));
}

void push(std::vector<Byte> &stack, double value) {
    const std::size_t n = sizeof(double);
    const Byte *bytes = reinterpret_cast<const Byte *>(&value);

    for (std::size_t i = 0; i < n; ++i) {
        stack.push_back(bytes[i]);
    }
    stack.push_back(static_cast<Byte>(n));
}

bool pop(std::vector<Byte> &stack, float &value) {
    const std::size_t n = sizeof(float);
    if (stack.empty()) {
        return false;
    }

    Byte tag = stack.back();
    if (tag != n || stack.size() < n + 1) {
        return false;
    }

    stack.pop_back();
    std::size_t start = stack.size() - n;
    std::memcpy(&value, &stack[start], n);
    stack.resize(start);
    return true;
}

bool pop(std::vector<Byte> &stack, double &value) {
    const std::size_t n = sizeof(double);
    if (stack.empty()) {
        return false;
    }

    Byte tag = stack.back();
    if (tag != n || stack.size() < n + 1) {
        return false;
    }

    stack.pop_back();
    std::size_t start = stack.size() - n;
    std::memcpy(&value, &stack[start], n);
    stack.resize(start);
    return true;
}

} // namespace object_representation
