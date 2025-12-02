#ifndef _HEADERS_H
#define _HEADERS_H

#include "BMP_types.h"

extern int readHeaders(void);

extern int getFileHeader(BITMAPFILEHEADER *h);

extern int getInfoHeader(BITMAPINFOHEADER *h);
#endif
// EOF
