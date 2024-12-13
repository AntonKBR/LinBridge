# **LinBridge**

**LinBridge** is an open-source project designed to bridge LIN and CAN signals, providing GPIO mappings for car functions. Initially built on an Arduino Pro Mini and TTL LIN adapter, the project is transitioning to an ESP32-based architecture with CAN translation, wireless updates, and a custom PCB design.

---

## **Features**

- **LIN Signal Translation**: Translate LIN signals to car-specific functions via GPIO mappings.
- **Backlight Adjustment**: Adjust brightness based on LIN inputs (e.g., Shift+/-, Shift -).
- **CAN Signal Translator (Planned)**: Enable CAN communication using a transceiver.
- **Wireless Updates (Planned)**: Support over-the-air firmware updates.
- **Custom PCB Design (Planned)**: Integrate ESP32, LIN, and CAN transceivers in a compact layout.

---

## **Supported Platforms**

- **VW MQB Platform**: Specifically designed to support LIN and CAN communication for the Volkswagen Modular Transverse Matrix (MQB) platform.

---

## **Hardware Requirements**

### **Current Setup**
- **Microcontroller**: Arduino Pro Mini
- **LIN Adapter**: TTL LIN adapter (e.g., MCP2004A)
- **GPIO Buttons**: Functions like Arrow Up, Arrow Down, OK, Shift+/-, etc.

### **Future Setup**
- **Microcontroller**: ESP32-C3-Zero or ESP32-based board
- **Transceivers**:
  - LIN: MCP2004A or TJA1020
  - CAN: MCP2515 or TJA1050 (Planned)
- **Custom PCB**: Compact, ESP32-integrated design (Planned).

---

## **Installation**

1. Clone the repository:
   ```bash
   git clone https://github.com/yourusername/LinBridge.git
   cd LinBridge