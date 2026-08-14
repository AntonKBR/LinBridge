#pragma once

#include <cstddef>
#include <cstdint>

#include "linbridge/lin/frame.h"

namespace linbridge::lin {

class Transport {
  public:
    virtual ~Transport() = default;

    virtual void begin() = 0;
    virtual void sendHeader(std::uint8_t identifier) = 0;
    virtual void sendFrame(std::uint8_t identifier, const std::uint8_t* data,
                           std::size_t length) = 0;
    virtual void clearReceive() = 0;
    virtual bool readByte(std::uint8_t& value) = 0;
};

}  // namespace linbridge::lin
