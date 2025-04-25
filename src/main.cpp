// Smart Home UI for ST7789 Display with Arduino UNO R3
// Utilizes cbm80amiga/ST7789 AVR library
// Optimized for ZERO flickering with direct screen updates

#include <Arduino.h>
#include <SPI.h>
#include <ST7789_AVR.h>
#include <display/config.h>
#include <display/display.h>

// Initialize display
ST7789_AVR tft(TFT_DC, TFT_RST, TFT_CS);

// Initialize display manager
Display display(tft);

// Sample data (replace with real data from sensors or external source)
float temperature = 23.5;
float humidity = 45.2;
float aqi = 35.0;
int ldrValue = 780;
bool motionDetected = false;
bool relay1 = true;
bool relay2 = false;
bool relay3 = true;
bool relay4 = false;

// Forward declarations
void updateSensorData();

void setup()
{
  // Initialize serial for debugging
  Serial.begin(9600);

  // Initialize SPI
  SPI.begin();
  SPI.setClockDivider(SPI_CLOCK_DIV2);

  // Initialize display
  display.init();

  // Set initial data
  display.setErrorMessage("System Error: WiFi connection lost");
  display.setInfoMessage("System Info: Data uploaded to cloud");
  display.setSuccessMessage("Success: System configuration updated");
  display.setWarningMessage("Warning: Battery level low");
  display.setSensorData(temperature, humidity, aqi);
  display.setDeviceData(ldrValue, motionDetected, relay1, relay2, relay3, relay4);
}

void loop()
{
  // Update sample data randomly
  updateSensorData();

  // Update display (handles screen transitions)
  display.update();

  // Small delay to prevent hogging the CPU
  delay(50);
}

void updateSensorData()
{
  // In a real application, this would read from actual sensors
  // For demo, simulate changing values slowly
  static unsigned long lastUpdate = 0;

  if (millis() - lastUpdate > 3000)
  { // Every 3 seconds
    if (random(100) < 20)
    { // 20% chance to update
      temperature += random(-10, 11) / 100.0;
      humidity += random(-10, 11) / 100.0;
      aqi += random(-5, 6) / 10.0;

      // Keep values in reasonable ranges
      temperature = constrain(temperature, 18.0, 30.0);
      humidity = constrain(humidity, 30.0, 70.0);
      aqi = constrain(aqi, 0.0, 150.0);

      // Update display with new values
      display.setSensorData(temperature, humidity, aqi);

      // Occasionally update device states
      if (random(100) < 10)
      {
        ldrValue = random(100, 1023);
        motionDetected = random(2) == 1;

        if (random(100) < 5)
        {
          // Toggle random relay
          int relay = random(4);
          switch (relay)
          {
          case 0:
            relay1 = !relay1;
            break;
          case 1:
            relay2 = !relay2;
            break;
          case 2:
            relay3 = !relay3;
            break;
          case 3:
            relay4 = !relay4;
            break;
          }
        }

        display.setDeviceData(ldrValue, motionDetected, relay1, relay2, relay3, relay4);
      }
    }

    lastUpdate = millis();
  }
}