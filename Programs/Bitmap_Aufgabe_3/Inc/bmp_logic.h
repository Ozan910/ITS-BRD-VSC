#ifndef BMP_LOGIC_H
#define BMP_LOGIC_H

#include "BMP_types.h"
#include <stdbool.h>

/**
 * @brief Hauptfunktion für Aufgabe C (und B)
Liest das Bild, skaliert es und gibt es zeilenweise aus.
 * * @param info Der Infoheader.
 * * @param palette die Farbpalette.
 */
void drawScaledImage(BITMAPINFOHEADER* info, RGBQUAD* palette);

/**
*@brief draws image without scaling or checking for size
*@param palette 256 * 4 Byte color palette
*@param infoheader BITMAPINFOHEADER
*/
void drawWithoutChecks(BITMAPINFOHEADER infoheader, RGBQUAD* palette);

#endif