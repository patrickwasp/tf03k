# TF03K Arduino Library

An Arduino library for interfacing with the TF03K battery monitor that provides real-time battery information including capacity percentage, voltage, current, and remaining time.

## Hardware Specifications

- **Baud Rate**: 9600bps
- **Data Format**: 16 bytes per message, sent once per second
- **Interface**: TTL level 3-5V
- **Power Requirements**: VCC and GND must be powered by external power (5-10mA)
- **Isolation**: Solid-state Optocoupler
- **Error Checking**: 8-bit cumulative sum from first byte to fifteenth byte

## Connection

The TF03K requires TTL level serial connection:
- RXD: TF03 receive pin (can be disconnected if only reading data)
- TXD: TF03 transmission pin
- GND: Ground
- VCC: Power supply voltage 3-5V, 5-10mA (Must be powered by external power)

**Important Notes:**
- VCC and GND must connect to external power
- The interface is NOT RS-232 compatible. It must be TTL level.

## Installation

1. Download this repository as a ZIP file
2. In Arduino IDE, go to Sketch -> Include Library -> Add .ZIP Library
3. Select the downloaded ZIP file

## Usage

Basic example:
```cpp
#include <TF03KSerial.h>

TF03KSerial batteryMonitor;

void setup() {
    Serial.begin(115200);  // For debug output
    Serial1.begin(9600);   // For TF03K communication
    batteryMonitor.begin(Serial1);

    // Optional: Set up message handler for data updates
    batteryMonitor.setMessageHandler([](float capacity_percent, float voltage, float capacity, float current, int remaining_seconds) {
        Serial.print("Capacity: ");
        Serial.print(capacity_percent);
        Serial.println("%");
        
        Serial.print("Voltage: ");
        Serial.print(voltage);
        Serial.println("V");
        
        Serial.print("Capacity: ");
        Serial.print(capacity);
        Serial.println("Ah");
        
        Serial.print("Current: ");
        Serial.print(current);
        Serial.println("A");
        
        Serial.print("Remaining Time: ");
        Serial.print(remaining_seconds);
        Serial.println("s");
    });
}

void loop() {
    batteryMonitor.update();  // Process any available data
    delay(10);
}
```

### Data Ranges

- Capacity Percentage: 0-100%
- Total Battery Voltage: 0-500.00V
- Real-time Battery Capacity: 0.1-5000Ah
- Current: -750000mA to +750000mA
- Remaining Time: 00:00:00 to 99:59:59

### Direct Value Access

You can also access the values directly:
```cpp
float voltage = batteryMonitor.getVoltage();
float current = batteryMonitor.getCurrent();
float capacity = batteryMonitor.getCapacity();
float capacityPercent = batteryMonitor.getCapacityPercent();
int remainingSeconds = batteryMonitor.getRemainingSeconds();
```

### Error Handling

The library provides error detection for:
- Buffer Overflow
- CRC Mismatch
- Invalid Frame

You can set up an error handler:
```cpp
batteryMonitor.setErrorHandler([](Tf03k::Error error) {
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
    }
});
```