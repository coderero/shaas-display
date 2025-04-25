#ifndef DISPLAY_H
#define DISPLAY_H

#include <ST7789_AVR.h>
#include "config.h"
#include "screens/message_screen.h"
#include "screens/sensor_screen.h"
#include "screens/devices_screen.h"

class Display
{
private:
    ST7789_AVR &tft;

    // Screen instances
    ErrorScreen errorScreen;
    InfoScreen infoScreen;
    SuccessScreen successScreen;
    WarningScreen warningScreen;
    SensorsScreen sensorsScreen;
    DevicesScreen devicesScreen;

    DisplayMode currentMode;
    unsigned long lastScreenChange;

public:
    Display(ST7789_AVR &display);

    void init();
    void update();
    void showSplash();

    // Set which screen to display
    void setMode(DisplayMode mode);

    // Update message text
    void setErrorMessage(const char *msg);
    void setInfoMessage(const char *msg);
    void setSuccessMessage(const char *msg);
    void setWarningMessage(const char *msg);

    // Update sensor data
    void setSensorData(float temp, float humidity, float aqi);

    // Update device data
    void setDeviceData(int ldr, bool motion, bool r1, bool r2, bool r3, bool r4);
};

#endif // DISPLAY_H