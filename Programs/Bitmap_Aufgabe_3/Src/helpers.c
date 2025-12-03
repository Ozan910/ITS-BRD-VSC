#include "helpers.h"

uint16_t rgb24ToRgb565(RGBQUAD color){
    uint16_t rgb565;
    rgb565 = (color.rgbRed >> 3) << 11;//>> 3 löscht die 3 LSBs, farbe wird ungenauer aber kleiner, << 11 schiebt die farbe in position weil 5bit rot ganz links sind
    rgb565 |= (color.rgbGreen >> 2) << 5;//>> löscht die 2 LSBs, << 5 schiebt gruen in position weil 6bit gruen in der mitte sind
    rgb565 |= (color.rgbBlue >> 3);//>> löscht die 3 LSBs, muss nicht geschoben werden weil blau ganz rechts ist
    return rgb565;
}