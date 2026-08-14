#include <cassert>
#include <cstdint>

#include "linbridge/lin/protocol.h"
#include "linbridge/lin/validator.h"

namespace {

using linbridge::lin::ChecksumModel;
using linbridge::lin::FrameSpec;
using linbridge::lin::RawFrame;
using linbridge::lin::ValidationCounters;
using linbridge::lin::ValidationError;

RawFrame makeSyntheticFrame(const FrameSpec& spec) {
    RawFrame frame;
    frame.push(linbridge::lin::kSyncByte);
    frame.push(spec.protectedIdentifier);
    frame.push(0x12);
    frame.push(0x34);

    const std::uint8_t* data = frame.bytes.data() + 2;
    const std::uint8_t checksum =
        spec.checksumModel == ChecksumModel::kEnhanced
            ? linbridge::lin::enhancedChecksum(spec.protectedIdentifier, data,
                                               spec.dataLength)
            : linbridge::lin::classicChecksum(data, spec.dataLength);
    frame.push(checksum);
    return frame;
}

}  // namespace

int main() {
    const FrameSpec spec{
        linbridge::lin::protectedIdentifier(0x0E),
        2,
        ChecksumModel::kEnhanced,
    };

    const RawFrame valid = makeSyntheticFrame(spec);
    assert(linbridge::lin::validate(valid, spec) == ValidationError::kNone);

    RawFrame truncated = valid;
    --truncated.length;
    assert(linbridge::lin::validate(truncated, spec) ==
           ValidationError::kTruncated);

    RawFrame overlong = valid;
    overlong.push(0x00);
    assert(linbridge::lin::validate(overlong, spec) ==
           ValidationError::kOverlong);

    RawFrame wrongSync = valid;
    wrongSync.bytes[0] = 0x00;
    assert(linbridge::lin::validate(wrongSync, spec) ==
           ValidationError::kWrongSync);

    RawFrame invalidParity = valid;
    invalidParity.bytes[1] ^= 0x40;
    assert(linbridge::lin::validate(invalidParity, spec) ==
           ValidationError::kInvalidPidParity);

    RawFrame unexpectedPid = valid;
    unexpectedPid.bytes[1] =
        linbridge::lin::protectedIdentifier(linbridge::lin::kAccIdentifier);
    assert(linbridge::lin::validate(unexpectedPid, spec) ==
           ValidationError::kUnexpectedPid);

    RawFrame invalidChecksum = valid;
    invalidChecksum.bytes[invalidChecksum.length - 1] ^= 0x01;
    assert(linbridge::lin::validate(invalidChecksum, spec) ==
           ValidationError::kInvalidChecksum);

    RawFrame overflow;
    for (std::size_t index = 0;
         index <= linbridge::lin::kMaximumCapturedFrameSize; ++index) {
        overflow.push(static_cast<std::uint8_t>(index));
    }
    assert(linbridge::lin::validate(overflow, spec) ==
           ValidationError::kBufferOverflow);

    ValidationCounters counters;
    counters.record(ValidationError::kNone);
    counters.record(ValidationError::kInvalidChecksum);
    assert(counters.accepted == 1);
    assert(counters.invalidChecksum == 1);

    const FrameSpec classicSpec{spec.protectedIdentifier, 2,
                                ChecksumModel::kClassic};
    const RawFrame classic = makeSyntheticFrame(classicSpec);
    assert(linbridge::lin::validate(classic, classicSpec) ==
           ValidationError::kNone);

    return 0;
}
