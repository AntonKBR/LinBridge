#pragma once

#include <cstdint>

#include "linbridge/lin/frame.h"

namespace linbridge::lin {

struct ButtonResponse {
    std::uint8_t sequenceCounter;
    std::uint8_t firstButtonCode;
    std::uint8_t secondButtonCode;

    constexpr bool isNeutral() const {
        return firstButtonCode == 0x00 && secondButtonCode == 0x00;
    }
};

struct AccResponse {
    // The purpose of data byte 0 is not established by the retained captures.
    std::uint8_t unknown0;
    std::uint8_t sequenceCounter;
    std::uint8_t buttonState;
    std::uint8_t controlState;

    constexpr bool isNeutral() const {
        return buttonState == 0x80 && controlState == 0x2B;
    }
};

bool decodeButtonResponse(const RawFrame& frame, ButtonResponse& response);
bool decodeAccResponse(const RawFrame& frame, AccResponse& response);

}  // namespace linbridge::lin
