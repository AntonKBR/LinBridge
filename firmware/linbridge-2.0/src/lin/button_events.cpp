#include "linbridge/lin/button_events.h"

namespace linbridge::lin {

void ButtonEventBatch::push(const ButtonSource source, const std::uint8_t code,
                            const bool pressed) {
    if (count < events.size()) {
        events[count++] = ButtonEvent{source, code, pressed};
    }
}

void LegacyButtonEventTracker::updateSlot(
    const std::uint8_t currentCode, const std::uint8_t neutralCode,
    std::uint8_t& pressedCode, const ButtonSource source,
    ButtonEventBatch& events) {
    if (currentCode != neutralCode) {
        if (pressedCode != currentCode) {
            events.push(source, currentCode, true);
            pressedCode = currentCode;
        }
    } else if (pressedCode != neutralCode) {
        events.push(source, pressedCode, false);
        pressedCode = neutralCode;
    }
}

ButtonEventBatch LegacyButtonEventTracker::process(
    const ButtonResponse& response, const std::uint32_t nowMillis) {
    ButtonEventBatch events;
    if (nowMillis < kLegacyStartupSuppressionMillis) {
        return events;
    }
    updateSlot(response.firstButtonCode, 0x00, firstButton_,
               ButtonSource::kFirstButtonSlot, events);
    updateSlot(response.secondButtonCode, 0x00, secondButton_,
               ButtonSource::kSecondButtonSlot, events);
    return events;
}

ButtonEventBatch LegacyButtonEventTracker::process(
    const AccResponse& response, const std::uint32_t nowMillis) {
    ButtonEventBatch events;
    if (nowMillis < kLegacyStartupSuppressionMillis) {
        return events;
    }
    updateSlot(response.buttonState, 0x80, accButton_, ButtonSource::kAcc,
               events);
    return events;
}

}  // namespace linbridge::lin
