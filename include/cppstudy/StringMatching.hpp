#pragma once
#include <string_view>
namespace string_matching {
std::size_t matchNaive(std::string_view text, std::string_view pattern);
std::size_t matchKMP(std::string_view text, std::string_view pattern);
} // namespace string_matching
