#pragma once

#include <cstdint>

#include "linbridge/lin/response.h"
#include "linbridge/lin/transport.h"
#include "linbridge/lin/validator.h"

namespace linbridge::lin {

enum class ResponseChannel : std::uint8_t {
    kNone,
    kButtons,
    kAcc,
};

enum class PollEvent : std::uint8_t {
    kNone,
    kResponseAccepted,
    kResponseRejected,
    kResponseTimeout,
};

struct PollResult {
    PollEvent event{PollEvent::kNone};
    ResponseChannel channel{ResponseChannel::kNone};
    ValidationError validationError{ValidationError::kNone};
    ButtonResponse buttons{};
    AccResponse acc{};
};

struct MasterCounters {
    std::uint32_t buttonTimeouts{0};
    std::uint32_t accTimeouts{0};
    ValidationCounters buttonValidation{};
    ValidationCounters accValidation{};
};

class Master {
  public:
    explicit Master(Transport& transport) : transport_(transport) {}

    void begin();
    void sendFixedIllumination();
    void requestButtons();
    void requestAcc();
    PollResult poll(std::uint32_t nowMillis);

    const MasterCounters& counters() const { return counters_; }

  private:
    enum class State : std::uint8_t {
        kSendIllumination,
        kSendButtonRequest,
        kAwaitButtonResponse,
        kSendAccRequest,
        kAwaitAccResponse,
    };

    void beginResponseWindow(State state, std::uint32_t nowMillis);
    void drainReceive();
    PollResult finishButtons();
    PollResult finishAcc();
    PollResult timeout(ResponseChannel channel);

    Transport& transport_;
    State state_{State::kSendIllumination};
    std::uint32_t responseDeadline_{0};
    RawFrame received_{};
    MasterCounters counters_{};
};

}  // namespace linbridge::lin
