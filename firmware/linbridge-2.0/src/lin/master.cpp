#include "linbridge/lin/master.h"

#include "linbridge/lin/protocol.h"

namespace linbridge::lin {

namespace {

constexpr std::size_t kResponseFrameLength = 11;

bool deadlineReached(const std::uint32_t now,
                     const std::uint32_t deadline) {
    return static_cast<std::int32_t>(now - deadline) >= 0;
}

}  // namespace

void Master::begin() {
    transport_.begin();
    received_.clear();
    state_ = State::kSendIllumination;
}

void Master::sendFixedIllumination() {
    transport_.sendFrame(kIlluminationIdentifier,
                         kFixedIlluminationPayload.data(),
                         kFixedIlluminationPayload.size());
}

void Master::requestButtons() {
    transport_.sendHeader(kButtonIdentifier);
}

void Master::requestAcc() { transport_.sendHeader(kAccIdentifier); }

void Master::beginResponseWindow(const State state,
                                 const std::uint32_t nowMillis) {
    transport_.clearReceive();
    received_.clear();
    state_ = state;
    responseDeadline_ = nowMillis + kResponseTimeoutMillis;
}

void Master::drainReceive() {
    std::uint8_t value = 0;
    while (transport_.readByte(value)) {
        received_.push(value);
    }
}

PollResult Master::finishButtons() {
    PollResult result;
    result.channel = ResponseChannel::kButtons;
    result.validationError = validate(received_, kButtonResponseSpec);
    counters_.buttonValidation.record(result.validationError);
    state_ = State::kSendAccRequest;

    if (result.validationError != ValidationError::kNone ||
        !decodeButtonResponse(received_, result.buttons)) {
        result.event = PollEvent::kResponseRejected;
        return result;
    }
    result.event = PollEvent::kResponseAccepted;
    return result;
}

PollResult Master::finishAcc() {
    PollResult result;
    result.channel = ResponseChannel::kAcc;
    result.validationError = validate(received_, kAccResponseSpec);
    counters_.accValidation.record(result.validationError);
    state_ = State::kSendIllumination;

    if (result.validationError != ValidationError::kNone ||
        !decodeAccResponse(received_, result.acc)) {
        result.event = PollEvent::kResponseRejected;
        return result;
    }
    result.event = PollEvent::kResponseAccepted;
    return result;
}

PollResult Master::timeout(const ResponseChannel channel) {
    PollResult result;
    result.event = PollEvent::kResponseTimeout;
    result.channel = channel;
    result.validationError = ValidationError::kTruncated;
    if (channel == ResponseChannel::kButtons) {
        ++counters_.buttonTimeouts;
        state_ = State::kSendAccRequest;
    } else {
        ++counters_.accTimeouts;
        state_ = State::kSendIllumination;
    }
    return result;
}

PollResult Master::poll(const std::uint32_t nowMillis) {
    switch (state_) {
        case State::kSendIllumination:
            transport_.clearReceive();
            sendFixedIllumination();
            state_ = State::kSendButtonRequest;
            return {};

        case State::kSendButtonRequest:
            beginResponseWindow(State::kAwaitButtonResponse, nowMillis);
            requestButtons();
            return {};

        case State::kAwaitButtonResponse:
            drainReceive();
            if (received_.overflow || received_.length >= kResponseFrameLength) {
                return finishButtons();
            }
            if (deadlineReached(nowMillis, responseDeadline_)) {
                if (received_.length > 0) {
                    return finishButtons();
                }
                return timeout(ResponseChannel::kButtons);
            }
            return {};

        case State::kSendAccRequest:
            beginResponseWindow(State::kAwaitAccResponse, nowMillis);
            requestAcc();
            return {};

        case State::kAwaitAccResponse:
            drainReceive();
            if (received_.overflow || received_.length >= kResponseFrameLength) {
                return finishAcc();
            }
            if (deadlineReached(nowMillis, responseDeadline_)) {
                if (received_.length > 0) {
                    return finishAcc();
                }
                return timeout(ResponseChannel::kAcc);
            }
            return {};
    }
    return {};
}

}  // namespace linbridge::lin
