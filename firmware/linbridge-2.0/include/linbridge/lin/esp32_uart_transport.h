#pragma once

#include <HardwareSerial.h>

#include "linbridge/lin/transport.h"

namespace linbridge::lin {

class Esp32UartTransport final : public Transport {
  public:
    explicit Esp32UartTransport(HardwareSerial& serial) : serial_(serial) {}

    void begin() override;
    void sendHeader(std::uint8_t identifier) override;
    void sendFrame(std::uint8_t identifier, const std::uint8_t* data,
                   std::size_t length) override;
    void clearReceive() override;
    bool readByte(std::uint8_t& value) override;

  private:
    void sendBreak();
    void writeHeader(std::uint8_t identifier);

    HardwareSerial& serial_;
};

}  // namespace linbridge::lin
