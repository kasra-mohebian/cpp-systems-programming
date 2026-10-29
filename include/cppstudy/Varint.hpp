#pragma once
#include <cstdint>
#include <vector>
namespace varint {
std::vector<std::uint8_t> compress(const std::vector<std::uint64_t> &data);
std::vector<std::uint64_t> decompress(const std::vector<std::uint8_t> &data);
} // namespace varint
