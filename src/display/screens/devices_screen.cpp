#include <display/screens/devices_screen.h>
#include <math.h>

DevicesScreen::DevicesScreen(ST7789_AVR &display)
    : BaseScreen(display), ldrValue(780), motionDetected(false),
      ldrSensorId("LDR_01"), motionSensorId("MOT_01")
{
    // Initialize relay states
    for (int i = 0; i < 6; i++)
    {
        relayStates[i] = false;
        // Default: first 4 are 10A (false), last 2 are 30A (true)
        heavyDutyRelays[i] = (i >= 4);
    }
}

void DevicesScreen::setSensorData(int ldr, bool motion)
{
    ldrValue = ldr;
    motionDetected = motion;
}

void DevicesScreen::setRelayStates(bool r1, bool r2, bool r3, bool r4, bool r5, bool r6)
{
    relayStates[0] = r1;
    relayStates[1] = r2;
    relayStates[2] = r3;
    relayStates[3] = r4;
    relayStates[4] = r5;
    relayStates[5] = r6;
}

void DevicesScreen::setRelayTypes(bool *heavyDutyFlags, int count)
{
    for (int i = 0; i < min(count, 6); i++)
    {
        heavyDutyRelays[i] = heavyDutyFlags[i];
    }
}

void DevicesScreen::setLdrSensorId(String id)
{
    ldrSensorId = id;
}

void DevicesScreen::setMotionSensorId(String id)
{
    motionSensorId = id;
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

    // Display LDR sensor ID
    tft.setTextColor(COLOR_WARNING);
    tft.setTextSize(1);
    tft.setCursor(45, 70);
    tft.print("ID: ");
    tft.print(ldrSensorId);

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

    // Display motion sensor ID
    tft.setTextColor(COLOR_INFO);
    tft.setTextSize(1);
    tft.setCursor(45, 120);
    tft.print("ID: ");
    tft.print(motionSensorId);

    // Relay states
    tft.setTextColor(COLOR_SUCCESS);
    tft.setTextSize(1);
    tft.setCursor(20, 150);
    tft.print("Relay States:");

    // Draw standard relay indicators (10A) - first 4
    for (int i = 0; i < 4; i++)
    {
        if (!heavyDutyRelays[i])
        {
            drawRelayStatus(30 + (i * 40), 170, relayStates[i], false);
            tft.setCursor(30 + (i * 40) + 5, 190);
            tft.print("10A");
        }
    }

    // Draw heavy duty relay indicators (30A) - last 2
    int heavyDutyCount = 0;
    for (int i = 0; i < 6; i++)
    {
        if (heavyDutyRelays[i])
        {
            drawRelayStatus(30 + (heavyDutyCount * 40), 210, relayStates[i], true);
            tft.setCursor(30 + (heavyDutyCount * 40) + 5, 190);
            tft.print("30A");
            heavyDutyCount++;
        }
    }
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

void DevicesScreen::drawRelayStatus(int x, int y, bool state, bool isHeavyDuty)
{
    uint16_t fillColor = state ? COLOR_SUCCESS : COLOR_ERROR;

    // Make heavy duty relays larger
    int width = isHeavyDuty ? 30 : 20;
    int height = isHeavyDuty ? 18 : 15;

    tft.fillRoundRect(x, y, width, height, 3, fillColor);
    tft.drawRoundRect(x, y, width, height, 3, COLOR_TEXT);

    // Add indicator for heavy duty
    if (isHeavyDuty)
    {
        tft.drawLine(x + 5, y + height - 5, x + width - 5, y + height - 5, COLOR_TEXT);
    }
}