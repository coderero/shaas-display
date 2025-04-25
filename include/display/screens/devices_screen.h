#ifndef DEVICES_SCREEN_H
#define DEVICES_SCREEN_H

#include "base_screen.h"

class DevicesScreen : public BaseScreen
{
private:
    int ldrValue;
    bool motionDetected;
    bool relay1;
    bool relay2;
    bool relay3;
    bool relay4;

    void drawSunIcon(int x, int y);
    void drawMotionIcon(int x, int y);
    void drawRelayStatus(int x, int y, bool state);

public:
    DevicesScreen(ST7789_AVR &tft);

    void setData(int ldr, bool motion, bool r1, bool r2, bool r3, bool r4);
    void draw() override;
};

#endif // DEVICES_SCREEN_H