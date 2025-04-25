#include <display/screens/devices_screen.h>
#include <math.h>

DevicesScreen::DevicesScreen(ST7789_AVR &display)
    : BaseScreen(display), ldrValue(780), motionDetected(false),
      relay1(true), relay2(false), relay3(true), relay4(false)
{
}

void DevicesScreen::setData(int ldr, bool motion, bool r1, bool r2, bool r3, bool r4)
{
    ldrValue = ldr;
    motionDetected = motion;
    relay1 = r1;
    relay2 = r2;
    relay3 = r3;
    relay4 = r4;
}

void DevicesScreen::draw()
{
    // Title bar
    tft.fillRect(0, 0, SCREEN_WIDTH, 30, COLOR_HIGHLIGHT);
    tft.setTextColor(COLOR_TEXT);
    tft.setTextSize(2);
    tft.setCursor(50, 10);
    tft.print("DEVICE STATUS");

    // Light sensor
    tft.setTextColor(COLOR_WARNING);
    tft.setTextSize(1);
    tft.setCursor(45, 50); // Moved right to align better with icon
    tft.print("Light level:");
    tft.setTextSize(2);
    tft.setCursor(180, 50);
    tft.print(map(ldrValue, 0, 1023, 0, 100));
    tft.print("%");

    // Draw sun icon with proper alignment
    drawSunIcon(20, 50);

    // Motion sensor
    tft.setTextColor(COLOR_INFO);
    tft.setTextSize(1);
    tft.setCursor(45, 100); // Moved right to align better with icon
    tft.print("Motion detected:");
    tft.setTextSize(2);
    tft.setCursor(180, 100);
    tft.print(motionDetected ? "YES" : "NO");

    // Draw motion icon with proper alignment
    drawMotionIcon(20, 100);

    // Relay states
    tft.setTextColor(COLOR_SUCCESS);
    tft.setTextSize(1);
    tft.setCursor(20, 150);
    tft.print("Relay States:");

    // Draw relay indicators
    drawRelayStatus(40, 170, relay1);
    drawRelayStatus(80, 170, relay2);
    drawRelayStatus(120, 170, relay3);
    drawRelayStatus(160, 170, relay4);

    // Labels
    tft.setTextSize(1);
    tft.setCursor(38, 190);
    tft.print("R1");
    tft.setCursor(78, 190);
    tft.print("R2");
    tft.setCursor(118, 190);
    tft.print("R3");
    tft.setCursor(158, 190);
    tft.print("R4");

    // Draw update time
    tft.setTextColor(COLOR_TEXT);
    tft.setTextSize(1);
    tft.setCursor(150, 220);
    tft.print("Updated: Now");
}

void DevicesScreen::drawSunIcon(int x, int y)
{
    // Draw sun icon with better alignment
    tft.drawCircle(x + 4, y + 4, 6, COLOR_WARNING);

    // Draw sun rays
    for (int i = 0; i < 8; i++)
    {
        float angle = i * PI / 4;
        int startX = x + 4 + round(cos(angle) * 8);
        int startY = y + 4 + round(sin(angle) * 8);
        int endX = x + 4 + round(cos(angle) * 12);
        int endY = y + 4 + round(sin(angle) * 12);

        tft.drawLine(startX, startY, endX, endY, COLOR_WARNING);
    }
}

void DevicesScreen::drawMotionIcon(int x, int y)
{
    if (motionDetected)
    {
        // Animated motion waves when motion is detected
        for (int i = 0; i < 3; i++)
        {
            tft.drawCircle(x + 4, y + 4, 4 + i * 3, COLOR_INFO);
        }
    }
    else
    {
        // Simple circle when no motion
        tft.drawCircle(x + 4, y + 4, 6, COLOR_INFO);
    }
}

void DevicesScreen::drawRelayStatus(int x, int y, bool state)
{
    uint16_t color = state ? COLOR_SUCCESS : COLOR_ERROR;
    tft.fillRoundRect(x, y, 20, 15, 3, color);
    tft.drawRoundRect(x, y, 20, 15, 3, COLOR_TEXT);
}