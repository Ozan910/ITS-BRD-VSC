#ifndef MAIN_H
#define MAIN_H

#include "BMP_types.h"
#include <stdint.h>
#include <stdbool.h>

/**
*@brief prints an image without checking for its size
*@param infoheader pointer on the infoheader of the current file
*@param palette pointer on the palette
*/
void drawOriginalSize(const BITMAPINFOHEADER* infoheader, const RGBQUAD* palette);

/** 
*@brief decodes the next line of the bitmap and writes it in the array thats given
*@param infoheader BITMAPINFOHEADER
*@param pixelLine Array where the decoded index of the pixels will be written
*@param cord coordinate to iterate through
*@return boolean if it there is another line
*/
bool decodeNextLine(const BITMAPINFOHEADER* infoheader, uint8_t* pixelLine, Coordinate* cord);

/**
*@brief prints an image after scaling it down. max 5x 480x320
*@param infoheader pointer on the infoheader of the current file
*@param palette pointer on the palette
*/
void scaleAndPrint(const BITMAPINFOHEADER* infoheader, const RGBQUAD* palette);
#endif
//eof