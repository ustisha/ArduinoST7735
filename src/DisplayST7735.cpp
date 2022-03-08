#include "DisplayST7735.h"
#include "washingMachine16.h"
#include "heater16.h"
#include "thermometer16.h"
#include "hygrometer16.h"
#include "lightOff.h"
#include "lightOn.h"

DisplayST7735::DisplayST7735(int8_t cs, int8_t dc, int8_t rst, uint8_t rotation)
{
    tft = new Adafruit_ST7735(cs, dc, rst);
    tft->initR(INITR_BLACKTAB);
    tft->fillScreen(ST77XX_BLACK);
    tft->setRotation(rotation);
}

void DisplayST7735::renderTempIcon(uint8_t x, uint8_t y)
{
    tft->drawRGBBitmap(x, y, thermometer16, TH_ICON_WIDTH, ICON_HEIGHT);
}

void DisplayST7735::renderHumIcon(uint8_t x, uint8_t y)
{
    tft->drawRGBBitmap(x, y, hygrometer16, TH_ICON_WIDTH, ICON_HEIGHT);
}

void DisplayST7735::renderHeaterIcon(uint8_t x, uint8_t y)
{
    tft->drawRGBBitmap(x, y, heater16, 15, ICON_HEIGHT);
}

void DisplayST7735::renderWashingMachineIcon(uint8_t x, uint8_t y)
{
    tft->drawRGBBitmap(x, y, washingMachine16, 14, ICON_HEIGHT);
}

void DisplayST7735::renderLamp(uint8_t x, uint8_t y, LightController *light)
{
    tft->fillRect(x, y, 36, 44, 0x000);
    if (light->getRelayState() == RELAY_ON) {
        tft->drawRGBBitmap(x, y, lightOn, 36, 44);
    } else {
        tft->drawRGBBitmap(x + 4, y + 3, lightOff, 28, 40);
    }
}

void DisplayST7735::tempUpdate(uint8_t x, uint8_t y, float data, uint8_t dec)
{
    tft->fillRect(x + TH_ICON_WIDTH + 2, y + 4, 20, 8, 0x000);
    tft->setCursor(x + TH_ICON_WIDTH + 2, y + 4);
    tft->setTextColor(0xF750, 0x000);
    tft->print(data, dec);
    tft->print("C");
}

void DisplayST7735::humUpdate(uint8_t x, uint8_t y, float data)
{
    tft->fillRect(x + TH_ICON_WIDTH + 2, y + 4, 20, 8, 0x000);
    tft->setCursor(x + TH_ICON_WIDTH + 2, y + 4);
    tft->setTextColor(0x5DFC, 0x000);
    tft->print(data, 0);
    tft->print("%");
}

void DisplayST7735::renderScale(uint8_t x, uint8_t y, uint8_t scalePercent)
{
    if (scalePercent >= 100) {
        scalePercent = 100;
        tft->setCursor(x, y);
    } else {
        tft->setCursor(x + 6, y);
    }
    tft->fillRect(x, y, 20, 8, 0x000);
    tft->setTextColor(0x5DFC, 0x000);
    tft->print(scalePercent);
    tft->print("%");
    uint8_t scaleStartX = x + 14;
    uint8_t scaleStartY = 16;
    uint8_t scaleWidth = 8;
    const uint8_t scaleHeight = 5;
    const uint8_t scaleSpace = 2;
    const uint8_t scaleItems = 7;
    const uint8_t filledStart = floor((double) scaleItems * scalePercent / 100);
    for (uint8_t i = 0; i < scaleItems; i++) {
        if (i >= (scaleItems - filledStart)) {
            tft->fillRect(scaleStartX, scaleStartY, scaleWidth, scaleHeight, 0x5DFC);
        } else {
            tft->fillRect(scaleStartX, scaleStartY, scaleWidth, scaleHeight, 0x000);
            tft->drawRect(scaleStartX, scaleStartY, scaleWidth, scaleHeight, 0x5DFC);
        }
        scaleStartX += 1;
        scaleWidth += -1;
        scaleStartY += (scaleHeight + scaleSpace);
    }
}

void DisplayST7735::lightInfo(const uint8_t x, const uint8_t y, LightController *light, LightUpdate *update)
{
    const uint8_t statusX4 = x + 26;
    const uint8_t statusX3 = x + 29;
    const uint8_t statusX2 = x + 34;
    const uint8_t statusX1 = x + 36;
    const uint8_t statusY = y + 6;
    const uint8_t rubberW = 25;
    const uint8_t offTimeout = light->getOffTime();
    if (update->value != light->getRelayState()) {
        renderLamp(x + 22, y + 16, light);
        update->value = light->getRelayState();
    }
    if (update->value != light->getRelayState()
        || update->mode != light->mode
        || (offTimeout == 0 && update->timeout != offTimeout)) {
        if (light->mode == MODE_AUTO) {
            tft->fillRect(statusX4, statusY, rubberW, 8, 0x000);
            tft->setCursor(statusX4, statusY);
            tft->setTextColor(0x5D4C, 0x000);
            tft->print("AUTO");
            update->mode = light->mode;
            update->timeout = offTimeout;
        } else if (light->mode == MODE_MANUAL) {
            if (light->getRelayState() == RELAY_ON) {
                tft->fillRect(statusX4, statusY, rubberW, 8, 0x000);
                tft->setTextColor(0xE000, 0x000);
                tft->setCursor(statusX2, statusY);
                tft->print("ON");
                update->value = light->getRelayState();
            }
            if (light->getRelayState() == RELAY_OFF) {
                tft->fillRect(statusX4, statusY, rubberW, 8, 0x000);
                tft->setTextColor(0xE000, 0x000);
                tft->setCursor(statusX3, statusY);
                tft->print("OFF");
                update->value = light->getRelayState();
            }
            update->mode = light->mode;
        }
    } else if (update->timeout != offTimeout) {
        tft->fillRect(statusX4, statusY, rubberW, 8, 0x000);
        tft->setTextColor(0xF628);
        tft->setCursor(offTimeout > 100 ? statusX3 : (offTimeout < 10 ? statusX1 : statusX2), statusY);
        tft->print(offTimeout);
        update->timeout = offTimeout;
    } else {
        update->timeout = 0;
    }

    if (update->energyLvl != light->energyLvl) {
        tft->setTextColor(0xFF61, 0x000);
        tft->setCursor(x + 4, statusY);
        if (light->energyLvl > 0) {
            tft->print("E");
            tft->print(light->energyLvl);
        } else {
            tft->print("  ");
        }
        update->energyLvl = light->energyLvl;
    }
}
