#include "linbridge/lin/esp32_uart_transport.h"

#include <Arduino.h>

#include "linbridge/board_config.h"
#include "linbridge/lin/protocol.h"

namespace linbridge::lin {

void Esp32UartTransport::begin() {
    serial_.begin(kBaudRate, SERIAL_8N1, board::pins::kLinRx,
                  board::pins::kLinTx);
}

void Esp32UartTransport::sendBreak() {
    pinMode(board::pins::kLinTx, OUTPUT);
    digitalWrite(board::pins::kLinTx, LOW);
    delayMicroseconds(kBreakDurationMicros);
    pinMode(board::pins::kLinTx, INPUT_PULLUP);

    // Preserve the UART reacquisition sequence used by the working prototype.
    begin();
}

void Esp32UartTransport::writeHeader(const std::uint8_t identifier) {
    sendBreak();
    serial_.write(kSyncByte);
    serial_.write(protectedIdentifier(identifier));
}

void Esp32UartTransport::sendHeader(const std::uint8_t identifier) {
    writeHeader(identifier);
    serial_.flush();
}

void Esp32UartTransport::sendFrame(const std::uint8_t identifier,
                                   const std::uint8_t* data,
                                   const std::size_t length) {
    writeHeader(identifier);
    serial_.write(data, length);
    serial_.write(
        enhancedChecksum(protectedIdentifier(identifier), data, length));
    serial_.flush();
}

}  // namespace linbridge::lin
