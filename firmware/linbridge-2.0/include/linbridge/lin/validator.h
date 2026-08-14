#pragma once

#include <cstdint>

#include "linbridge/lin/frame.h"

namespace linbridge::lin {

enum class ValidationError : std::uint8_t {
    kNone,
    kBufferOverflow,
    kTruncated,
    kOverlong,
    kWrongSync,
    kInvalidPidParity,
    kUnexpectedPid,
    kInvalidChecksum,
};

ValidationError validate(const RawFrame& frame, const FrameSpec& spec);

struct ValidationCounters {
    std::uint32_t accepted{0};
    std::uint32_t bufferOverflow{0};
    std::uint32_t truncated{0};
    std::uint32_t overlong{0};
    std::uint32_t wrongSync{0};
    std::uint32_t invalidPidParity{0};
    std::uint32_t unexpectedPid{0};
    std::uint32_t invalidChecksum{0};

    void record(ValidationError error);
};

}  // namespace linbridge::lin
