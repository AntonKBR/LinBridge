#pragma once

#include <array>
#include <cstddef>
#include <cstdint>

#include "linbridge/lin/response.h"

namespace linbridge::lin {

inline constexpr std::uint32_t kLegacyStartupSuppressionMillis = 1'000;

enum class ButtonSource : std::uint8_t {
    kFirstButtonSlot,
    kSecondButtonSlot,
    kAcc,
};

struct ButtonEvent {
    ButtonSource source;
    std::uint8_t code;
    bool pressed;
};

struct ButtonEventBatch {
    std::array<ButtonEvent, 2> events{};
    std::size_t count{0};

    void push(ButtonSource source, std::uint8_t code, bool pressed);
};

// Reproduces the state transitions in the retained working legacy parser.
// Output routing is intentionally outside this class.
class LegacyButtonEventTracker {
  public:
    ButtonEventBatch process(const ButtonResponse& response,
                             std::uint32_t nowMillis);
    ButtonEventBatch process(const AccResponse& response,
                             std::uint32_t nowMillis);

  private:
    static void updateSlot(std::uint8_t currentCode, std::uint8_t neutralCode,
                           std::uint8_t& pressedCode, ButtonSource source,
                           ButtonEventBatch& events);

    std::uint8_t firstButton_{0x00};
    std::uint8_t secondButton_{0x00};
    std::uint8_t accButton_{0x80};
};

}  // namespace linbridge::lin
