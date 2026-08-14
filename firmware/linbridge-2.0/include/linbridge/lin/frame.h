#pragma once

#include <array>
#include <cstddef>
#include <cstdint>

namespace linbridge::lin {

inline constexpr std::size_t kMaximumCapturedFrameSize = 16;

struct RawFrame {
    std::array<std::uint8_t, kMaximumCapturedFrameSize> bytes{};
    std::size_t length{0};
    bool overflow{false};

    void clear() {
        length = 0;
        overflow = false;
    }

    bool push(const std::uint8_t value) {
        if (length >= bytes.size()) {
            overflow = true;
            return false;
        }
        bytes[length++] = value;
        return true;
    }
};

enum class ChecksumModel : std::uint8_t {
    kClassic,
    kEnhanced,
};

struct FrameSpec {
    std::uint8_t protectedIdentifier;
    std::size_t dataLength;
    ChecksumModel checksumModel;
};

}  // namespace linbridge::lin
