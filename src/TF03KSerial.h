#ifndef TF03K_SERIAL_H_
#define TF03K_SERIAL_H_

#include <Arduino.h>
#include "tf03k.h"

class TF03KSerial {
 public:
  TF03KSerial();
  
  // Arduino-specific methods
  void begin(Stream& serial);
  void update();
  
  // Wrapper methods to access the underlying TF03K instance
  void setErrorHandler(void (*handler)(Tf03k::ErrorType));
  void setMessageHandler(void (*handler)(float capacity_percent, float voltage,
                                       float capacity, float current,
                                       int remaining_seconds));
  
  // Getter methods
  float getCapacityPercent() { return _tf03k.capacity_percent; }
  float getVoltage() { return _tf03k.voltage; }
  float getCapacity() { return _tf03k.capacity; }
  float getCurrent() { return _tf03k.current; }
  int getRemainingSeconds() { return _tf03k.remaining_seconds; }

 private:
  Tf03k _tf03k;
  Stream* _serial;
};

#endif  // TF03K_SERIAL_H_ 