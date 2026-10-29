#include "cppstudy/StringMatching.hpp"
#include <vector>
using namespace std;
namespace string_matching {
size_t matchNaive(string_view text, string_view pattern)

{
    const size_t n = text.size();
    const size_t m = pattern.size();

    if (m == 0) {
        return 0;
    }
    if (m > n) {
        return string_view::npos;
    }

    for (size_t i = 0; i <= n - m; ++i) {
        size_t j = 0;
        while (j < m && text[i + j] == pattern[j]) {
            ++j;
        }
        if (j == m) {
            return i;
        }
    }
    return string_view::npos;
}
size_t matchKMP(string_view text, string_view pattern) {
    const size_t n = text.size();
    const size_t m = pattern.size();

    if (m == 0) {
        return 0;
    }
    if (m > n) {
        return string_view::npos;
    }

    vector<size_t> lps(m, 0);

    // build LPS table
    size_t len = 0;
    for (size_t i = 1; i < m;) {
        if (pattern[i] == pattern[len]) {
            lps[i] = ++len;
            ++i;
        } else if (len != 0) {
            len = lps[len - 1];
        } else {
            lps[i] = 0;
            ++i;
        }
    }

    // search
    size_t i = 0;
    size_t j = 0;
    while (i < n) {
        if (text[i] == pattern[j]) {
            ++i;
            ++j;
            if (j == m) {
                return i - j;
            }
        } else if (j != 0) {
            j = lps[j - 1];
        } else {
            ++i;
        }
    }

    return string_view::npos;
}
} // namespace string_matching
