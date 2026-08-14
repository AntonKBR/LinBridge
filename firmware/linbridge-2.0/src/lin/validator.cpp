#include "linbridge/lin/validator.h"

#include "linbridge/lin/protocol.h"

namespace linbridge::lin {

ValidationError validate(const RawFrame& frame, const FrameSpec& spec) {
    if (frame.overflow) {
        return ValidationError::kBufferOverflow;
    }

    constexpr std::size_t kHeaderLength = 2;
    constexpr std::size_t kChecksumLength = 1;
    const std::size_t expectedLength =
        kHeaderLength + spec.dataLength + kChecksumLength;

    if (frame.length < expectedLength) {
        return ValidationError::kTruncated;
    }
    if (frame.length > expectedLength) {
        return ValidationError::kOverlong;
    }
    if (frame.bytes[0] != kSyncByte) {
        return ValidationError::kWrongSync;
    }

    const std::uint8_t actualPid = frame.bytes[1];
    if (protectedIdentifier(actualPid & 0x3F) != actualPid) {
        return ValidationError::kInvalidPidParity;
    }
    if (actualPid != spec.protectedIdentifier) {
        return ValidationError::kUnexpectedPid;
    }

    const std::uint8_t* data = frame.bytes.data() + kHeaderLength;
    const std::uint8_t actualChecksum = frame.bytes[expectedLength - 1];
    const std::uint8_t expectedChecksum =
        spec.checksumModel == ChecksumModel::kEnhanced
            ? enhancedChecksum(actualPid, data, spec.dataLength)
            : classicChecksum(data, spec.dataLength);

    return actualChecksum == expectedChecksum ? ValidationError::kNone
                                              : ValidationError::kInvalidChecksum;
}

void ValidationCounters::record(const ValidationError error) {
    switch (error) {
        case ValidationError::kNone:
            ++accepted;
            break;
        case ValidationError::kBufferOverflow:
            ++bufferOverflow;
            break;
        case ValidationError::kTruncated:
            ++truncated;
            break;
        case ValidationError::kOverlong:
            ++overlong;
            break;
        case ValidationError::kWrongSync:
            ++wrongSync;
            break;
        case ValidationError::kInvalidPidParity:
            ++invalidPidParity;
            break;
        case ValidationError::kUnexpectedPid:
            ++unexpectedPid;
            break;
        case ValidationError::kInvalidChecksum:
            ++invalidChecksum;
            break;
    }
}

}  // namespace linbridge::lin
