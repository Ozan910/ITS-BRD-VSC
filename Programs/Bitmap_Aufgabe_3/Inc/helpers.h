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

/**
*@brief prints a pixel on the lcd of the board using drawPixel
*@param palette the array of the colors of the palette
*@param pixelIndex what color of the palette is used
*@param cord where to print the pixel
*/
void drawPixelWithPalette(RGBQUAD* palette, uint8_t pixelIndex, Coordinate cord);

/**
*@brief Prints a line of pixel at the same time using GUI Write Line
*@param palette colors of the palette
*@param pixelLine array of the index of the pixels that will be printed
*@param colorLine array will be used to store the colors of every pixel
*@param width number of pixels in a line
*@param cord first pixel of the line
* */
void drawLine(const RGBQUAD* palette, uint8_t* pixelLine, uint16_t* colorLine,uint32_t width, Coordinate cord);

#endif
//EOF