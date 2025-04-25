#ifndef SENSORS_SCREEN_H
#define SENSORS_SCREEN_H

#include "base_screen.h"

class SensorsScreen : public BaseScreen
{
private:
    float temperature;
    float humidity;
    float aqi;
    String sensorId; // Add sensor ID field

    void drawTemperatureIcon(int x, int y);
    void drawHumidityIcon(int x, int y);
    void drawAqiIndicator(int x, int y, int width, int height);

public:
    SensorsScreen(ST7789_AVR &tft);

    void setData(float temp, float hum, float airQuality);
    void setSensorId(String id); // Setter for sensor ID
    void draw() override;
};

#endif // SENSORS_SCREEN_H