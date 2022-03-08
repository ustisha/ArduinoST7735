#ifndef DISPLAYST7735_H
#define DISPLAYST7735_H

//#define SERIAL_DEBUG

#include <Arduino.h>
#include <DebugLog.h>
#include <Net.h>
#include <DisplayView.h>
#include <Adafruit_ST7735.h>

class DisplayST7735 : public DisplayView {
public:

    DisplayST7735(int8_t cs, int8_t dc, int8_t rst, uint8_t rotation = 1);

protected:

    const uint8_t TH_ICON_WIDTH = 8;
    const uint8_t ICON_HEIGHT = 16;

    Adafruit_ST7735 *tft;

    void renderTempIcon(uint8_t x, uint8_t y);

    void renderHeaterIcon(uint8_t x, uint8_t y);

    void renderHumIcon(uint8_t x, uint8_t y);

    void renderWashingMachineIcon(uint8_t x, uint8_t y);

    void renderLamp(uint8_t x, uint8_t y, LightController *light);

    void renderScale(uint8_t x, uint8_t y, uint8_t scalePercent);

    void tempUpdate(uint8_t x, uint8_t y, float data, uint8_t dec = 0);

    void humUpdate(uint8_t x, uint8_t y, float data);

    void lightInfo(uint8_t x, uint8_t y, LightController *light, LightUpdate *update);
};

#endif //DISPLAYST7735_H
