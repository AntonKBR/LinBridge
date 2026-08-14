#pragma once

#include <array>
#include <cstddef>
#include <cstdint>

namespace linbridge::board {

using Gpio = std::uint8_t;

namespace pins {

inline constexpr Gpio kLinTx = 1;
inline constexpr Gpio kLinRx = 2;
inline constexpr Gpio kKey1Sda = 3;
inline constexpr Gpio kKey1Scl = 4;
inline constexpr Gpio kCcsSwitchInput = 5;
inline constexpr Gpio kIlluminationMeasurement = 6;
inline constexpr Gpio kCanRx = 7;
inline constexpr Gpio kCanTx = 8;
inline constexpr Gpio kMfaResetInput = 9;
inline constexpr Gpio kMfaDownInput = 10;
inline constexpr Gpio kMfaUpInput = 11;
inline constexpr Gpio kCcsMinusInput = 12;
inline constexpr Gpio kCcsPlusInput = 13;

}  // namespace pins

// Current hardware scope: one resistive output channel and no dedicated LIN
// sleep-control GPIO. USB is used for programming and host diagnostics.
inline constexpr bool kKey2Implemented = false;
inline constexpr bool kDedicatedLinSleepControlImplemented = false;
inline constexpr bool kUsbProgrammingAndDiagnostics = true;

inline constexpr std::array<Gpio, 13> kAssignedPins = {
    pins::kLinTx,
    pins::kLinRx,
    pins::kKey1Sda,
    pins::kKey1Scl,
    pins::kCcsSwitchInput,
    pins::kIlluminationMeasurement,
    pins::kCanRx,
    pins::kCanTx,
    pins::kMfaResetInput,
    pins::kMfaDownInput,
    pins::kMfaUpInput,
    pins::kCcsMinusInput,
    pins::kCcsPlusInput,
};

constexpr bool assignedPinsAreUnique() {
    for (std::size_t first = 0; first < kAssignedPins.size(); ++first) {
        for (std::size_t second = first + 1; second < kAssignedPins.size();
             ++second) {
            if (kAssignedPins[first] == kAssignedPins[second]) {
                return false;
            }
        }
    }
    return true;
}

constexpr bool assignedPinsAreInDocumentedRange() {
    for (const Gpio pin : kAssignedPins) {
        if (pin < 1 || pin > 13) {
            return false;
        }
    }
    return true;
}

static_assert(assignedPinsAreUnique(), "LinBridge 2.0 GPIO assignments overlap");
static_assert(assignedPinsAreInDocumentedRange(),
              "GPIO14-GPIO17 are not part of the documented board mapping");
static_assert(!kKey2Implemented, "KEY2 is not implemented on the current board");
static_assert(!kDedicatedLinSleepControlImplemented,
              "No dedicated LIN sleep-control GPIO is documented");

}  // namespace linbridge::board
