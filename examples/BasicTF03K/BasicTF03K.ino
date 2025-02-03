#include <TF03KSerial.h>

TF03KSerial batteryMonitor;

void setup() {
  Serial.begin(115200);  // Initialize main serial for debug output
  Serial1.begin(9600);   // Initialize Serial1 for TF03K communication - 9600 baud
  
  batteryMonitor.begin(Serial1);
  
  // Set up message handler
  batteryMonitor.setMessageHandler([](float capacity_percent, float voltage,
                                    float capacity, float current,
                                    int remaining_seconds) {
    // Print the battery information
    Serial.print("Capacity: "); Serial.print(capacity_percent); Serial.println("%");
    Serial.print("Voltage: "); Serial.print(voltage); Serial.println("V");
    Serial.print("Capacity: "); Serial.print(capacity); Serial.println("Ah");
    Serial.print("Current: "); Serial.print(current); Serial.println("A");
    Serial.print("Remaining Time: "); Serial.print(remaining_seconds); Serial.println("s");
    Serial.println();
  });
  
  // Set up error handler
  batteryMonitor.setErrorHandler([](Tf03k::ErrorType error) {
    Serial.print("Error: ");
    switch (error) {
      case Tf03k::BufferOverflow:
        Serial.println("Buffer Overflow");
        break;
      case Tf03k::CRCMismatch:
        Serial.println("CRC Mismatch");
        break;
      case Tf03k::InvalidFrame:
        Serial.println("Invalid Frame");
        break;
      default:
        Serial.println("Unknown Error");
    }
  });
}

void loop() {
  batteryMonitor.update();  // Process any available data
  delay(10);  // Small delay to prevent overwhelming the serial buffer
} 