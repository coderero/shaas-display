#include <display/screens/sensor_screen.h>

SensorsScreen::SensorsScreen(ST7789_AVR &display)
    : BaseScreen(display), temperature(23.5), humidity(45.2), aqi(35.0)
{
}

void SensorsScreen::setData(float temp, float hum, float airQuality)
{
    temperature = temp;
    humidity = hum;
    aqi = airQuality;
}

void SensorsScreen::draw()
{
    // Title bar
    tft.fillRect(0, 0, SCREEN_WIDTH, 30, COLOR_INFO);
    tft.setTextColor(COLOR_TEXT);
    tft.setTextSize(2);
    tft.setCursor(50, 10);
    tft.print("SENSOR DATA");

    // Temperature
    uint16_t tempColor = 0xF811; // Red-orange for temperature
    tft.setTextColor(tempColor);
    tft.setTextSize(1);
    tft.setCursor(20, 50); // Moved right to align better with icon
    tft.print("Temperature:");
    tft.setTextSize(2);
    tft.setCursor(140, 50);
    tft.print(temperature, 1);
    tft.print(" C");

    // Draw temperature icon - adjusted position
    drawTemperatureIcon(20, 60);

    // Humidity
    uint16_t humColor = 0x0F5F; // Blue for humidity
    tft.setTextColor(humColor);
    tft.setTextSize(1);
    tft.setCursor(20, 120); // Moved right to align better with icon
    tft.print("Humidity:");
    tft.setTextSize(2);
    tft.setCursor(140, 120);
    tft.print(humidity, 1);
    tft.print(" %");

    // Draw humidity icon - adjusted position
    drawHumidityIcon(20, 136);

    // Air Quality Index
    uint16_t aqiBaseColor = 0x0770; // Green for AQI
    tft.setTextColor(aqiBaseColor);
    tft.setTextSize(1);
    tft.setCursor(20, 180); // Moved right to align with other icons
    tft.print("Air Quality:");
    tft.setTextSize(2);
    tft.setCursor(140, 180);
    tft.print(aqi, 1);

    // Draw AQI indicator
    drawAqiIndicator(20, 200, 80, 10);

    // Draw update time
    tft.setTextColor(COLOR_TEXT);
    tft.setTextSize(1);
    tft.setCursor(150, 220);
    tft.print("Updated: Now");
}

void SensorsScreen::drawTemperatureIcon(int x, int y)
{
    uint16_t tempColor = 0xF811;
    // Thermometer bulb and stem
    tft.fillRoundRect(x, y, 8, 18, 3, tempColor);
    tft.fillCircle(x + 4, y + 25, 6, tempColor);
}

void SensorsScreen::drawHumidityIcon(int x, int y)
{
    uint16_t humColor = 0x0F5F;
    // Water drop shape
    for (int i = 0; i < 3; i++)
    {
        tft.drawCircle(x + 4, y + 5, 6 - i, humColor);
    }
}

void SensorsScreen::drawAqiIndicator(int x, int y, int width, int height)
{
    // AQI color indicator based on value
    uint16_t aqiColor;
    if (aqi < 50)
        aqiColor = 0x07E0; // Good - Green
    else if (aqi < 100)
        aqiColor = 0xFFE0; // Moderate - Yellow
    else if (aqi < 150)
        aqiColor = 0xFD20; // Unhealthy - Orange
    else
        aqiColor = 0xF800; // Very Unhealthy - Red

    // Draw AQI indicator background
    tft.fillRect(x, y, width, height, COLOR_BG);

    // Fill bar according to AQI value (capped at 150)
    int fillWidth = int(min(aqi, 150.0f) * width / 150.0f);
    tft.fillRect(x, y, fillWidth, height, aqiColor);

    // Draw border
    tft.drawRect(x, y, width, height, COLOR_TEXT);
}