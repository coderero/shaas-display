#include <display/display.h>

Display::Display(ST7789_AVR &display)
    : tft(display),
      errorScreen(display),
      infoScreen(display),
      successScreen(display),
      warningScreen(display),
      sensorsScreen(display),
      devicesScreen(display),
      currentMode(MODE_INFO),
      lastScreenChange(0)
{
}

void Display::init()
{
    // Initialize display
    tft.init();
    tft.setRotation(2); // Adjust if needed

    // Show splash screen
    showSplash();

    // Draw initial screen
    tft.fillScreen(COLOR_BG);
    setMode(currentMode);
    lastScreenChange = millis();
}

void Display::update()
{
    // Check if it's time to change screens
    if (millis() - lastScreenChange > SCREEN_DURATION)
    {
        // Move to next screen
        currentMode = static_cast<DisplayMode>((currentMode + 1) % MODE_COUNT);

        // Clear screen and draw new content
        tft.fillScreen(COLOR_BG);
        setMode(currentMode);

        // Reset timer
        lastScreenChange = millis();
    }
}

void Display::showSplash()
{
    // Clear screen and draw splash
    tft.fillScreen(COLOR_BG);

    // Draw logo
    int centerX = SCREEN_WIDTH / 2;
    int centerY = SCREEN_HEIGHT / 2 - 20;

    // Draw circular logo
    for (int r = 0; r < 40; r += 2)
    {
        uint16_t color = r < 20 ? COLOR_INFO : COLOR_HIGHLIGHT;
        tft.drawCircle(centerX, centerY, r, color);
        delay(20); // Animation effect
    }

    // Draw text
    tft.setTextColor(COLOR_TEXT);
    tft.setTextSize(2);
    tft.setCursor(20, centerY + 50);
    tft.print("Smart Home System");
    tft.setTextSize(1);
    tft.setCursor(80, centerY + 80);
    tft.print("Initializing...");

    delay(1000);
}

void Display::setMode(DisplayMode mode)
{
    currentMode = mode;

    // Draw the appropriate screen
    switch (mode)
    {
    case MODE_ERROR:
        errorScreen.draw();
        break;
    case MODE_INFO:
        infoScreen.draw();
        break;
    case MODE_SUCCESS:
        successScreen.draw();
        break;
    case MODE_WARNING:
        warningScreen.draw();
        break;
    case MODE_SENSORS:
        sensorsScreen.draw();
        break;
    case MODE_DEVICES:
        devicesScreen.draw();
        break;
    default:
        break;
    }
}

void Display::setErrorMessage(const char *msg)
{
    errorScreen.setMessage(msg);
    if (currentMode == MODE_ERROR)
    {
        tft.fillScreen(COLOR_BG);
        errorScreen.draw();
    }
}

void Display::setInfoMessage(const char *msg)
{
    infoScreen.setMessage(msg);
    if (currentMode == MODE_INFO)
    {
        tft.fillScreen(COLOR_BG);
        infoScreen.draw();
    }
}

void Display::setSuccessMessage(const char *msg)
{
    successScreen.setMessage(msg);
    if (currentMode == MODE_SUCCESS)
    {
        tft.fillScreen(COLOR_BG);
        successScreen.draw();
    }
}

void Display::setWarningMessage(const char *msg)
{
    warningScreen.setMessage(msg);
    if (currentMode == MODE_WARNING)
    {
        tft.fillScreen(COLOR_BG);
        warningScreen.draw();
    }
}

void Display::setSensorData(float temp, float humidity, float aqi)
{
    sensorsScreen.setData(temp, humidity, aqi);
    if (currentMode == MODE_SENSORS)
    {
        tft.fillScreen(COLOR_BG);
        sensorsScreen.draw();
    }
}

void Display::setDeviceData(int ldr, bool motion, bool r1, bool r2, bool r3, bool r4)
{
    devicesScreen.setData(ldr, motion, r1, r2, r3, r4);
    if (currentMode == MODE_DEVICES)
    {
        tft.fillScreen(COLOR_BG);
        devicesScreen.draw();
    }
}