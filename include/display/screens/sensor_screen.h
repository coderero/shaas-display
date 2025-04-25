#ifndef SENSORS_SCREEN_H
#define SENSORS_SCREEN_H

#include "base_screen.h"

class SensorsScreen : public BaseScreen
{
private:
    float temperature;
    float humidity;
    float aqi;

    void drawTemperatureIcon(int x, int y);
    void drawHumidityIcon(int x, int y);
    void drawAqiIndicator(int x, int y, int width, int height);

public:
    SensorsScreen(ST7789_AVR &tft);

    void setData(float temp, float humidity, float airQuality);
    void draw() override;
};

#endif // SENSORS_SCREEN_H