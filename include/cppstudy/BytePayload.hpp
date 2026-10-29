#pragma once
#include <array>
#include <cstddef>
namespace hashtable {
// Public-edition replacement for a course-provided value type.
class BytePayload {
    std::array<std::byte, 64> bytes_{};

  public:
    BytePayload() = default;
    BytePayload(BytePayload &&) noexcept = default;
    BytePayload &operator=(BytePayload &&) noexcept = default;
    BytePayload(const BytePayload &) = delete;
    BytePayload &operator=(const BytePayload &) = delete;
    std::byte *data() noexcept { return bytes_.data(); }
    const std::byte *data() const noexcept { return bytes_.data(); }
};
} // namespace hashtable
