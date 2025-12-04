#include "helpers.h"
#include "LCD_GUI.h"
uint16_t rgb24ToRgb565(RGBQUAD color){
    uint16_t rgb565;
    rgb565 = (color.rgbRed >> 3) << 11;//>> 3 löscht die 3 LSBs, farbe wird ungenauer aber kleiner, << 11 schiebt die farbe in position weil 5bit rot ganz links sind
    rgb565 |= (color.rgbGreen >> 2) << 5;//>> löscht die 2 LSBs, << 5 schiebt gruen in position weil 6bit gruen in der mitte sind
    rgb565 |= (color.rgbBlue >> 3);//>> löscht die 3 LSBs, muss nicht geschoben werden weil blau ganz rechts ist
    return rgb565;
}

void drawPixelWithPalette(RGBQUAD* palette, uint8_t pixelIndex, Coordinate cord){
    RGBQUAD color24 = palette[pixelIndex];//farbe aus palette entnehmen
    uint16_t color565 = rgb24ToRgb565(color24);//in RGB 565 umwandeln für LCD
    GUI_drawPoint(cord, color565, DOT_PIXEL_1X1, DOT_FILL_AROUND);
}

void drawLine(const RGBQUAD* palette, uint8_t* pixelLine, uint16_t* colorLine,uint32_t width, Coordinate cord){
    for(uint32_t x = 0; x < width; x++){
        colorLine[x] = rgb24ToRgb565(palette[pixelLine[x]]);
    }
    GUI_WriteLine(cord, width, colorLine);
}