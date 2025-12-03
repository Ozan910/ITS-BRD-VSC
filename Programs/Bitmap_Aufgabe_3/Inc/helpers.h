#ifndef HELPERS_H
#define HELPERS_H

#include "BMP_types.h"
#include <stdint.h>

/**
*@brief converts a RGB24 Color to RGB565 by shifting the bits
*@param RGBQUAD the RGB24 Color to be converted
*@return the RGB565 color in uint16_t
 */
uint16_t rgb24ToRgb565(RGBQUAD color);

#endif
//EOF