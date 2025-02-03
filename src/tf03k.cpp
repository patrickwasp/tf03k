#include "tf03k.h"

// clang-format off
/*
+----------+-------------------------------+----------+-------------------------+---------------------------------+
| Byte Num | Meaning                       | Hex      | Example                  | Range                           |
+----------+-------------------------------+----------+-------------------------+---------------------------------+
| 1        | Index bytes                   | A5       | A5                      | A5                              |
| 2        | Capacity percentage           | 02       | 0x02 = 2%               | 0-100%                          |
| 3, 4     | Total battery voltage (V)     | 07 D0    | 0x07D0 = 2000 = 20.00V  | 0-500.00V                       |
| 5-8      | Real time battery capacity    | 00 00    | 0x00000A87 = 2695mAh =  | 0.1 - 5000Ah                    |
|          | (mAh)                         | 0A 87    | 2.695Ah                 |                                 |
| 9-12     | Current (mA)                  | 00 00    | 0x00002405 = 9221mAh =  | -750000mA - +750000mA           |
|          |                               | 24 05    | 9.221Ah                 |                                 |
| 13-15    | Remaining time (S)            | 00 94 11 | 0x009411 = 37905S =     | 00:00:00 - 99:59:59             |
|          |                               |          | 10H:31M:45S             |                                 |
| 16       | Check                         | Varied   | Depends on data         | Based on cumulative sum         |
+----------+-------------------------------+----------+-------------------------+---------------------------------+
*/

// clang-format on
Tf03k::Tf03k() {
  float capacity_percent = 0.0f;
  float voltage = 0.0f;
  float capacity = 0.0f;
  float current = 0.0f;
  int remaining_seconds = 0;
  reset();
}

void Tf03k::processByte(uint8_t b) {
  static bool start_seen = false;
  static uint8_t count_since_start = 0;

  if (!start_seen && b == START_BYTE) {
    start_seen = true;
    frame_length = 0;
    count_since_start = 0;
  }

  if (start_seen) {
    if (frame_length >= MAX_MESSAGE_LENGTH) {
      errorType = BufferOverflow;
      if (errorHandler) errorHandler(errorType);
      reset();
      start_seen = false;
      return;
    }

    frame_buffer[frame_length++] = b;
    count_since_start++;

    if (count_since_start == 16) {
      handleFrame();
      reset();
      start_seen = false;
      count_since_start = 0;
    }
  }
}

void Tf03k::handleFrame() {
  uint8_t computedCrc = calcCRC8(frame_buffer, frame_length - 1);

  if (computedCrc != frame_buffer[frame_length - 1]) {
    errorType = CRCMismatch;
    if (errorHandler) errorHandler(errorType);
    return;
  }

  capacity_percent = static_cast<float>(frame_buffer[1]);

  voltage = (static_cast<float>(static_cast<uint16_t>(frame_buffer[2]) << 8 |
                                frame_buffer[3])) /
            voltageConversionFactor;

  capacity = (static_cast<float>(static_cast<uint32_t>(frame_buffer[4]) << 24 |
                                 static_cast<uint32_t>(frame_buffer[5]) << 16 |
                                 static_cast<uint32_t>(frame_buffer[6]) << 8 |
                                 static_cast<uint32_t>(frame_buffer[7]))) /
             currentConversionFactor;

  current = (static_cast<float>(static_cast<int32_t>(
                (static_cast<uint32_t>(frame_buffer[8]) << 24) |
                (static_cast<uint32_t>(frame_buffer[9]) << 16) |
                (static_cast<uint32_t>(frame_buffer[10]) << 8) |
                static_cast<uint32_t>(frame_buffer[11])))) /
            currentConversionFactor;

  if (messageHandler) {
    messageHandler(capacity_percent, voltage, capacity, current,
                   remaining_seconds);
  }
}

void Tf03k::reset() {
  frame_length = 0;
  errorType = NoError;
}

void Tf03k::setErrorHandler(void (*handler)(ErrorType)) {
  errorHandler = handler;
}

void Tf03k::setMessageHandler(void (*handler)(float capacity_percent,
                                              float voltage, float capacity,
                                              float current,
                                              int remaining_seconds)) {
  messageHandler = handler;
}

uint8_t Tf03k::calcCRC8(const uint8_t* data, size_t length) {
  uint8_t crc = 0;
  for (size_t i = 0; i < length; ++i) {
    crc += data[i];
  }
  return crc;
}