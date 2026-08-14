#include <cassert>
#include <cstdint>

#include "linbridge/lin/protocol.h"
#include "linbridge/lin/response.h"
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

template <std::size_t Size>
RawFrame makeCapturedFrame(const std::uint8_t (&bytes)[Size]) {
    RawFrame frame;
    for (const std::uint8_t value : bytes) {
        assert(frame.push(value));
    }
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

    // Retained bench captures, reduced to protocol test vectors. Each frame is
    // sync + PID + eight data bytes + enhanced checksum.
    const std::uint8_t idleButtonBytes[] = {
        0x55, 0x8E, 0x10, 0x00, 0x00, 0x00,
        0x90, 0x00, 0x00, 0x00, 0xD0,
    };
    const RawFrame idleButton = makeCapturedFrame(idleButtonBytes);
    assert(linbridge::lin::validate(idleButton,
                                    linbridge::lin::kButtonResponseSpec) ==
           ValidationError::kNone);
    linbridge::lin::ButtonResponse buttonResponse{};
    assert(linbridge::lin::decodeButtonResponse(idleButton, buttonResponse));
    assert(buttonResponse.sequenceCounter == 0x10);
    assert(buttonResponse.isNeutral());
    assert(buttonResponse.firstPressType == 0x00);
    assert(buttonResponse.wheelType == 0x90);
    assert(buttonResponse.secondPressType == 0x00);
    assert(buttonResponse.paddleState == 0x00);
    assert(buttonResponse.auxiliaryState == 0x00);

    // Captured OK press with the right (+) paddle. This protects the complete
    // eight-byte 0x90 response layout, not only the two legacy button slots.
    const std::uint8_t okWithRightPaddleBytes[] = {
        0x55, 0x8E, 0x1C, 0x07, 0x00, 0x01,
        0x90, 0x00, 0x02, 0x00, 0xBA,
    };
    const RawFrame okWithRightPaddle =
        makeCapturedFrame(okWithRightPaddleBytes);
    assert(linbridge::lin::decodeButtonResponse(okWithRightPaddle,
                                                buttonResponse));
    assert(buttonResponse.sequenceCounter == 0x1C);
    assert(buttonResponse.firstButtonCode == 0x07);
    assert(buttonResponse.secondButtonCode == 0x00);
    assert(buttonResponse.firstPressType == 0x01);
    assert(buttonResponse.wheelType == 0x90);
    assert(buttonResponse.secondPressType == 0x00);
    assert(buttonResponse.paddleState == 0x02);
    assert(buttonResponse.auxiliaryState == 0x00);

    // The retained real-car A3 capture also uses enhanced checksum. An older
    // research note labels this frame "classic", but executable arithmetic
    // proves that claim wrong: classic would produce 0x4C, not 0xBD.
    const std::uint8_t a3IdleBytes[] = {
        0x55, 0x8E, 0x10, 0x00, 0x00, 0x00,
        0xA3, 0x00, 0x00, 0x00, 0xBD,
    };
    const RawFrame a3Idle = makeCapturedFrame(a3IdleBytes);
    const FrameSpec a3ClassicButtonSpec{
        linbridge::lin::protectedIdentifier(linbridge::lin::kButtonIdentifier),
        linbridge::lin::kButtonResponseDataLength,
        ChecksumModel::kClassic,
    };
    assert(linbridge::lin::validate(a3Idle,
                                    linbridge::lin::kButtonResponseSpec) ==
           ValidationError::kNone);
    assert(linbridge::lin::validate(a3Idle, a3ClassicButtonSpec) ==
           ValidationError::kInvalidChecksum);

    const std::uint8_t idleAccBytes[] = {
        0x55, 0xCF, 0xD2, 0x40, 0x80, 0x2B,
        0x00, 0x00, 0x00, 0x00, 0x71,
    };
    const RawFrame idleAcc = makeCapturedFrame(idleAccBytes);
    assert(linbridge::lin::validate(idleAcc,
                                    linbridge::lin::kAccResponseSpec) ==
           ValidationError::kNone);
    linbridge::lin::AccResponse accResponse{};
    assert(linbridge::lin::decodeAccResponse(idleAcc, accResponse));
    assert(accResponse.unknown0 == 0xD2);
    assert(accResponse.sequenceCounter == 0x40);
    assert(accResponse.isNeutral());

    const std::uint8_t activeAccBytes[] = {
        0x55, 0xCF, 0x21, 0x46, 0xB0, 0x2B,
        0x00, 0x00, 0x00, 0x00, 0xEC,
    };
    const RawFrame activeAcc = makeCapturedFrame(activeAccBytes);
    assert(linbridge::lin::decodeAccResponse(activeAcc, accResponse));
    assert(accResponse.sequenceCounter == 0x46);
    assert(accResponse.buttonState == 0xB0);
    assert(!accResponse.isNeutral());

    RawFrame corruptedCapture = activeAcc;
    corruptedCapture.bytes[corruptedCapture.length - 1] ^= 0x01;
    assert(!linbridge::lin::decodeAccResponse(corruptedCapture, accResponse));

    return 0;
}
