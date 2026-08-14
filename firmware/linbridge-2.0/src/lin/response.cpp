#include "linbridge/lin/response.h"

#include "linbridge/lin/protocol.h"
#include "linbridge/lin/validator.h"

namespace linbridge::lin {

namespace {

constexpr std::size_t kDataOffset = 2;

}  // namespace

bool decodeButtonResponse(const RawFrame& frame, ButtonResponse& response) {
    if (validate(frame, kButtonResponseSpec) != ValidationError::kNone) {
        return false;
    }

    response.sequenceCounter = frame.bytes[kDataOffset];
    response.firstButtonCode = frame.bytes[kDataOffset + 1];
    response.secondButtonCode = frame.bytes[kDataOffset + 2];
    response.firstPressType = frame.bytes[kDataOffset + 3];
    response.wheelType = frame.bytes[kDataOffset + 4];
    response.secondPressType = frame.bytes[kDataOffset + 5];
    response.paddleState = frame.bytes[kDataOffset + 6];
    response.auxiliaryState = frame.bytes[kDataOffset + 7];
    return true;
}

bool decodeAccResponse(const RawFrame& frame, AccResponse& response) {
    if (validate(frame, kAccResponseSpec) != ValidationError::kNone) {
        return false;
    }

    response.unknown0 = frame.bytes[kDataOffset];
    response.sequenceCounter = frame.bytes[kDataOffset + 1];
    response.buttonState = frame.bytes[kDataOffset + 2];
    response.controlState = frame.bytes[kDataOffset + 3];
    return true;
}

}  // namespace linbridge::lin
