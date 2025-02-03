#ifndef TF03K_H_
#define TF03K_H_

#include <cstdint>
#include <cstring>

class Tf03k {
 public:
  enum ErrorType { NoError, BufferOverflow, CRCMismatch, InvalidFrame };

  Tf03k();
  void processByte(uint8_t b);
  void setErrorHandler(void (*errorHandler)(ErrorType));
  void setMessageHandler(void (*handler)(float capacity_percent, float voltage,
                                         float capacity, float current,
                                         int remaining_seconds));
  float capacity_percent;
  float voltage;
  float capacity;
  float current;
  int remaining_seconds;

 private:
  static const int START_BYTE = 0xA5;
  static const int MAX_MESSAGE_LENGTH = 17;
  static const int voltageConversionFactor = 100;
  static const int currentConversionFactor = 1000;

  uint8_t frame_buffer[MAX_MESSAGE_LENGTH];
  int frame_length;
  uint8_t message[MAX_MESSAGE_LENGTH];
  uint8_t crc;  // CRC value for error checking

  ErrorType errorType;

  uint8_t calcCRC8(const uint8_t* data, size_t length);

  void (*errorHandler)(ErrorType);
  void (*messageHandler)(float capacity_percent, float voltage, float capacity,
                         float current, int remaining_seconds);

  void reset();
  void handleFrame();
};

#endif  // TF03K_H_
