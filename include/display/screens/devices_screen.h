#ifndef DEVICES_SCREEN_H
#define DEVICES_SCREEN_H

#include "base_screen.h"

class DevicesScreen : public BaseScreen
{
private:
    int ldrValue;
    bool motionDetected;

    // Relay states
    bool relayStates[6];     // Increased to 6 relays total
    bool heavyDutyRelays[6]; // Track which relays are heavy duty (30A)

    // Sensor IDs
    int ldrSensorId;
    int motionSensorId;

    void drawSunIcon(int x, int y);
    void drawMotionIcon(int x, int y);
    void drawRelayStatus(int x, int y, bool state, bool isHeavyDuty);

public:
    DevicesScreen(ST7789_AVR &tft);

    // Updated to include 6 relays
    void setRelayStates(bool r1, bool r2, bool r3, bool r4, bool r5, bool r6);

    // Set relay types (which ones are heavy duty 30A)
    void setRelayTypes(bool *heavyDutyFlags, int count);

    // Set sensor data
    void setSensorData(int ldr, bool motion);

    // Set sensor IDs
    void setLdrSensorId(int id);
    void setMotionSensorId(int id);

    void draw() override;
};

#endif // DEVICES_SCREEN_H