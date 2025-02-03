#include <TF03KSerial.h>

TF03KSerial batteryMonitor;
unsigned long lastPrintTime = 0;
const unsigned long PRINT_INTERVAL = 1000;  // Print every 1 second

void setup() {
  Serial.begin(115200);  // Initialize main serial for debug output
  Serial1.begin(9600);   // Initialize Serial1 for TF03K communication - 9600 baud
  
  batteryMonitor.begin(Serial1);
  
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

void printBatteryInfo() {
  // Print the battery information using getter methods
  Serial.print("Capacity: "); Serial.print(batteryMonitor.getCapacityPercent()); Serial.println("%");
  Serial.print("Voltage: "); Serial.print(batteryMonitor.getVoltage()); Serial.println("V");
  Serial.print("Capacity: "); Serial.print(batteryMonitor.getCapacity()); Serial.println("Ah");
  Serial.print("Current: "); Serial.print(batteryMonitor.getCurrent()); Serial.println("A");
  Serial.print("Remaining Time: "); Serial.print(batteryMonitor.getRemainingSeconds()); Serial.println("s");
  Serial.println();
}

void loop() {
  batteryMonitor.update();  // Process any available data
  
  // Check if it's time to print the battery info
  unsigned long currentTime = millis();
  if (currentTime - lastPrintTime >= PRINT_INTERVAL) {
    printBatteryInfo();
    lastPrintTime = currentTime;
  }
  
  delay(10);  // Small delay to prevent overwhelming the serial buffer
} 