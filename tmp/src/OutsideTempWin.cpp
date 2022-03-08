#include "../../include/DisplaySSD1289/OutsideTempWin.h"

OutsideTempWin::OutsideTempWin(UTFT *d, uint16_t x, uint16_t y, uint16_t w, uint16_t h, const char *title)
        : THPWindow(d, x, y, w, h, title) {

}
