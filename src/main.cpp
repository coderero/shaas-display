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

// Environmental sensor ID
String envSensorId = "ENV_S01";

// Device sensor IDs
String ldrSensorId = "LDR_A42";
String motionSensorId = "MOT_B18";

// Expanded relay configuration (6 relays total)
bool relayStates[6] = {true, false, true, false, true, false};
bool heavyDutyRelays[6] = {false, false, false, false, true, true}; // Last 2 are 30A

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

  // Set initial notification messages
  display.setErrorMessage("System Error: WiFi connection lost");
  display.setInfoMessage("System Info: Data uploaded to cloud");
  display.setSuccessMessage("Success: System configuration updated");
  display.setWarningMessage("Warning: Battery level low");

  // Set environmental sensor data with ID
  display.setSensorData(temperature, humidity, aqi);
  display.setSensorId(envSensorId);

  // Set device data including sensor IDs
  display.setDeviceData(ldrValue, motionDetected,
                        relayStates[0], relayStates[1], relayStates[2],
                        relayStates[3], relayStates[4], relayStates[5]);
  display.setLdrSensorId(ldrSensorId);
  display.setMotionSensorId(motionSensorId);
  display.setRelayTypes(heavyDutyRelays, 6);
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
          int relay = random(6); // Updated to handle 6 relays
          relayStates[relay] = !relayStates[relay];
        }

        // Update device data with all 6 relay states
        display.setDeviceData(ldrValue, motionDetected,
                              relayStates[0], relayStates[1], relayStates[2],
                              relayStates[3], relayStates[4], relayStates[5]);
      }

      // Occasionally update sensor IDs (for demo purposes)
      if (random(100) < 2)
      {
        // Generate new random sensor IDs
        char envBuffer[10];
        sprintf(envBuffer, "ENV_%03d", (int)random(1, 100));
        envSensorId = String(envBuffer);

        char ldrBuffer[10];
        sprintf(ldrBuffer, "LDR_%03d", (int)random(1, 100));
        ldrSensorId = String(ldrBuffer);

        char motBuffer[10];
        sprintf(motBuffer, "MOT_%03d", (int)random(1, 100));
        motionSensorId = String(motBuffer);

        // Update sensor IDs
        display.setSensorId(envSensorId);
        display.setLdrSensorId(ldrSensorId);
        display.setMotionSensorId(motionSensorId);
      }
    }

    lastUpdate = millis();
  }
}