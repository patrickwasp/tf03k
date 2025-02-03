#include "TF03KSerial.h"

TF03KSerial::TF03KSerial() : _serial(nullptr) {}

void TF03KSerial::begin(Stream& serial) {
  _serial = &serial;
}

void TF03KSerial::update() {
  if (_serial && _serial->available()) {
    while (_serial->available()) {
      _tf03k.processByte(_serial->read());
    }
  }
}

void TF03KSerial::setErrorHandler(void (*handler)(Tf03k::ErrorType)) {
  _tf03k.setErrorHandler(handler);
}

void TF03KSerial::setMessageHandler(void (*handler)(float capacity_percent, 
                                                   float voltage,
                                                   float capacity, 
                                                   float current,
                                                   int remaining_seconds)) {
  _tf03k.setMessageHandler(handler);
} 