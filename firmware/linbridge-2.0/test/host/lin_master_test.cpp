#include <array>
#include <cassert>
#include <cstddef>
#include <cstdint>

#include "linbridge/lin/master.h"
#include "linbridge/lin/protocol.h"

namespace {

using linbridge::lin::PollEvent;
using linbridge::lin::RawFrame;
using linbridge::lin::ResponseChannel;

class FakeTransport final : public linbridge::lin::Transport {
  public:
    void begin() override { began = true; }

    void sendHeader(const std::uint8_t identifier) override {
        lastHeader = identifier;
        if (identifier == linbridge::lin::kButtonIdentifier && serveButtons) {
            enqueue(buttonFrame);
        } else if (identifier == linbridge::lin::kAccIdentifier && serveAcc) {
            enqueue(accFrame);
        }
    }

    void sendFrame(const std::uint8_t identifier, const std::uint8_t*,
                   const std::size_t length) override {
        lastFrameIdentifier = identifier;
        lastFrameLength = length;
        ++framesSent;
    }

    void clearReceive() override {
        receiveLength = 0;
        receiveIndex = 0;
    }

    bool readByte(std::uint8_t& value) override {
        if (receiveIndex >= receiveLength) {
            return false;
        }
        value = receiveBuffer[receiveIndex++];
        return true;
    }

    template <std::size_t Size>
    void enqueue(const std::array<std::uint8_t, Size>& bytes) {
        for (const std::uint8_t value : bytes) {
            receiveBuffer[receiveLength++] = value;
        }
    }

    bool began{false};
    bool serveButtons{true};
    bool serveAcc{true};
    std::uint8_t lastHeader{0};
    std::uint8_t lastFrameIdentifier{0};
    std::size_t lastFrameLength{0};
    std::uint32_t framesSent{0};

    std::array<std::uint8_t, 11> buttonFrame{
        0x55, 0x8E, 0x10, 0x00, 0x00, 0x00,
        0x90, 0x00, 0x00, 0x00, 0xD0,
    };
    std::array<std::uint8_t, 11> accFrame{
        0x55, 0xCF, 0x21, 0x46, 0xB0, 0x2B,
        0x00, 0x00, 0x00, 0x00, 0xEC,
    };

  private:
    std::array<std::uint8_t, 32> receiveBuffer{};
    std::size_t receiveLength{0};
    std::size_t receiveIndex{0};
};

}  // namespace

int main() {
    FakeTransport transport;
    linbridge::lin::Master master(transport);
    master.begin();
    assert(transport.began);

    assert(master.poll(0).event == PollEvent::kNone);
    assert(transport.framesSent == 1);
    assert(transport.lastFrameIdentifier ==
           linbridge::lin::kIlluminationIdentifier);
    assert(transport.lastFrameLength == 4);

    assert(master.poll(1).event == PollEvent::kNone);
    assert(transport.lastHeader == linbridge::lin::kButtonIdentifier);
    assert(master.poll(2).event == PollEvent::kNone);
    const linbridge::lin::PollResult buttons = master.poll(41);
    assert(buttons.event == PollEvent::kResponseAccepted);
    assert(buttons.channel == ResponseChannel::kButtons);
    assert(buttons.buttons.sequenceCounter == 0x10);
    assert(buttons.buttons.isNeutral());

    assert(master.poll(42).event == PollEvent::kNone);
    assert(transport.lastHeader == linbridge::lin::kAccIdentifier);
    assert(master.poll(43).event == PollEvent::kNone);
    const linbridge::lin::PollResult acc = master.poll(82);
    assert(acc.event == PollEvent::kResponseAccepted);
    assert(acc.channel == ResponseChannel::kAcc);
    assert(acc.acc.sequenceCounter == 0x46);
    assert(!acc.acc.isNeutral());
    assert(master.counters().buttonValidation.accepted == 1);
    assert(master.counters().accValidation.accepted == 1);

    transport.serveButtons = false;
    assert(master.poll(83).event == PollEvent::kNone);
    assert(master.poll(84).event == PollEvent::kNone);
    const linbridge::lin::PollResult timeout = master.poll(124);
    assert(timeout.event == PollEvent::kResponseTimeout);
    assert(timeout.channel == ResponseChannel::kButtons);
    assert(master.counters().buttonTimeouts == 1);

    transport.accFrame.back() ^= 0x01;
    assert(master.poll(125).event == PollEvent::kNone);
    assert(master.poll(126).event == PollEvent::kNone);
    const linbridge::lin::PollResult rejected = master.poll(165);
    assert(rejected.event == PollEvent::kResponseRejected);
    assert(rejected.channel == ResponseChannel::kAcc);
    assert(rejected.validationError ==
           linbridge::lin::ValidationError::kInvalidChecksum);
    assert(master.counters().accValidation.invalidChecksum == 1);

    return 0;
}
