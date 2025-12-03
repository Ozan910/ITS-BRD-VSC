#ifndef HELPERS_H
#define HELPERS_H

#include "BMP_types.h"
#include <stdint.h>
#include "LCD_general.h"

/**
*@brief converts a RGB24 Color to RGB565 by shifting the bits
*@param RGBQUAD the RGB24 Color to be converted
*@return the RGB565 color in uint16_t
 */
uint16_t rgb24ToRgb565(RGBQUAD color);

void drawPixelWithPalette(RGBQUAD* palette, uint8_t pixelIndex, Coordinate cord);

#endif
//EOF