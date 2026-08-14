#pragma once

#include <cstddef>
#include <cstdint>

namespace linbridge::lin {

class Transport {
  public:
    virtual ~Transport() = default;

    virtual void begin() = 0;
    virtual void sendHeader(std::uint8_t identifier) = 0;
    virtual void sendFrame(std::uint8_t identifier, const std::uint8_t* data,
                           std::size_t length) = 0;
};

}  // namespace linbridge::lin
