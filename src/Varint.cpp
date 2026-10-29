#include "cppstudy/Varint.hpp"
#include <stdexcept>
using namespace std;
namespace varint {
vector<uint8_t> compress(const vector<uint64_t> &data)
// Compress some data
{
    vector<uint8_t> result;

    for (uint64_t value : data) {
        while (true) {
            uint8_t byte = static_cast<uint8_t>(value & 0x7Fu);
            value >>= 7;

            if (value != 0) {
                byte |= 0x80u;
            }

            result.push_back(byte);

            if (value == 0) {
                break;
            }
        }
    }

    return result;
}
vector<uint64_t> decompress(const vector<uint8_t> &data)
// Decompress some data
{
    vector<uint64_t> result;

    uint64_t current = 0;
    unsigned shift = 0;

    for (uint8_t byte : data) {
        if (shift == 63 && (byte & 0xFEu))
            throw std::overflow_error("varint exceeds uint64");
        current |= (static_cast<uint64_t>(byte & 0x7Fu) << shift);

        if ((byte & 0x80u) == 0) {
            result.push_back(current);
            current = 0;
            shift = 0;
        } else {
            shift += 7;
            if (shift >= 64)
                throw std::overflow_error("varint exceeds uint64");
        }
    }

    if (shift != 0)
        throw std::invalid_argument("truncated varint");

    return result;
}
} // namespace varint
//---------------------------------------------------------------------------
