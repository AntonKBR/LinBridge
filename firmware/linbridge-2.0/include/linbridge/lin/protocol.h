#pragma once

#include <array>
#include <cstddef>
#include <cstdint>

namespace linbridge::lin {

inline constexpr std::uint32_t kBaudRate = 19'200;
inline constexpr std::uint32_t kBreakDurationMicros = 1'000;
inline constexpr std::uint32_t kResponseTimeoutMillis = 40;
inline constexpr std::uint8_t kSyncByte = 0x55;

// Six-bit LIN identifiers. The transmitted protected identifiers are derived
// from these values instead of being stored as unrelated magic constants.
inline constexpr std::uint8_t kIlluminationIdentifier = 0x0D;
inline constexpr std::uint8_t kButtonIdentifier = 0x0E;
inline constexpr std::uint8_t kAccIdentifier = 0x0F;

inline constexpr std::uint8_t kFixedIllumination = 0x64;
inline constexpr std::array<std::uint8_t, 4> kFixedIlluminationPayload = {
    kFixedIllumination,
    0xFF,
    0xFF,
    0xFF,
};

constexpr std::uint8_t protectedIdentifier(const std::uint8_t identifier) {
    const std::uint8_t id = identifier & 0x3F;
    const std::uint8_t p0 =
        ((id >> 0) ^ (id >> 1) ^ (id >> 2) ^ (id >> 4)) & 0x01;
    const std::uint8_t p1 =
        (~((id >> 1) ^ (id >> 3) ^ (id >> 4) ^ (id >> 5))) & 0x01;
    return id | (p0 << 6) | (p1 << 7);
}

constexpr std::uint8_t enhancedChecksum(const std::uint8_t pid,
                                        const std::uint8_t* data,
                                        const std::size_t length) {
    std::uint16_t sum = pid;
    for (std::size_t index = 0; index < length; ++index) {
        sum += data[index];
        if (sum > 0xFF) {
            sum = (sum & 0xFF) + 1;
        }
    }
    return static_cast<std::uint8_t>(~sum);
}

static_assert(protectedIdentifier(kIlluminationIdentifier) == 0x0D);
static_assert(protectedIdentifier(kButtonIdentifier) == 0x8E);
static_assert(protectedIdentifier(kAccIdentifier) == 0xCF);
static_assert(enhancedChecksum(protectedIdentifier(kIlluminationIdentifier),
                              kFixedIlluminationPayload.data(),
                              kFixedIlluminationPayload.size()) == 0x8E);

}  // namespace linbridge::lin
