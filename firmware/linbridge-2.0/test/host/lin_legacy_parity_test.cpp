#include <cassert>
#include <cstdint>

#include "linbridge/lin/button_events.h"

namespace {

using linbridge::lin::AccResponse;
using linbridge::lin::ButtonEventBatch;
using linbridge::lin::ButtonResponse;
using linbridge::lin::ButtonSource;
using linbridge::lin::LegacyButtonEventTracker;

void expectEvent(const ButtonEventBatch& batch, const std::size_t index,
                 const ButtonSource source, const std::uint8_t code,
                 const bool pressed) {
    assert(index < batch.count);
    assert(batch.events[index].source == source);
    assert(batch.events[index].code == code);
    assert(batch.events[index].pressed == pressed);
}

}  // namespace

int main() {
    LegacyButtonEventTracker tracker;

    // The retained parser ignored button output during its first second.
    ButtonResponse buttons{0x01, 0x04, 0x00};
    assert(tracker.process(buttons, 999).count == 0);

    // A still-held button becomes pressed when the startup gate opens.
    ButtonEventBatch events = tracker.process(buttons, 1'000);
    assert(events.count == 1);
    expectEvent(events, 0, ButtonSource::kFirstButtonSlot, 0x04, true);

    // Repeated frames for a held button do not repeat the output event.
    buttons.sequenceCounter = 0x02;
    assert(tracker.process(buttons, 1'080).count == 0);

    // Returning the slot to zero releases the code that was held.
    buttons.firstButtonCode = 0x00;
    events = tracker.process(buttons, 1'160);
    assert(events.count == 1);
    expectEvent(events, 0, ButtonSource::kFirstButtonSlot, 0x04, false);

    // The two legacy button slots are independent and may change together.
    buttons.firstButtonCode = 0x05;
    buttons.secondButtonCode = 0x07;
    events = tracker.process(buttons, 1'240);
    assert(events.count == 2);
    expectEvent(events, 0, ButtonSource::kFirstButtonSlot, 0x05, true);
    expectEvent(events, 1, ButtonSource::kSecondButtonSlot, 0x07, true);
    buttons.firstButtonCode = 0x00;
    buttons.secondButtonCode = 0x00;
    events = tracker.process(buttons, 1'320);
    assert(events.count == 2);
    expectEvent(events, 0, ButtonSource::kFirstButtonSlot, 0x05, false);
    expectEvent(events, 1, ButtonSource::kSecondButtonSlot, 0x07, false);

    // ACC uses 0x80, rather than zero, as the legacy neutral value.
    AccResponse acc{0x21, 0x46, 0x82, 0x2B};
    events = tracker.process(acc, 1'400);
    assert(events.count == 1);
    expectEvent(events, 0, ButtonSource::kAcc, 0x82, true);
    acc.sequenceCounter = 0x47;
    assert(tracker.process(acc, 1'480).count == 0);
    acc.buttonState = 0x80;
    events = tracker.process(acc, 1'560);
    assert(events.count == 1);
    expectEvent(events, 0, ButtonSource::kAcc, 0x82, false);

    // Preserve the exact legacy direct-transition behavior: A -> B emits a
    // press for B without synthesizing a release for A in the same slot.
    buttons.firstButtonCode = 0x04;
    assert(tracker.process(buttons, 1'640).count == 1);
    buttons.firstButtonCode = 0x05;
    events = tracker.process(buttons, 1'720);
    assert(events.count == 1);
    expectEvent(events, 0, ButtonSource::kFirstButtonSlot, 0x05, true);

    return 0;
}
