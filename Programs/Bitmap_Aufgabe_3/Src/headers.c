#include "headers.h"
#include "errorhandler.h"
#include "input.h"
#include <stdbool.h>
#include <stdlib.h>

static BITMAPFILEHEADER fileHeader;
static BITMAPINFOHEADER infoHeader;
static bool validHeaders   = false;

static int basicChecks(PBITMAPFILEHEADER  headerP, PBITMAPINFOHEADER infoP){
   if (NULL != headerP){
      RETURN_NOK_ON_ERR(headerP->bfType != BMP_SIGNATURE, "basisChecks: wrong signature.");
      RETURN_NOK_ON_ERR(headerP->bfOffBits >= headerP->bfSize, "basisChecks: bfOffBits points out of BMP file.");
   }
   if (NULL != infoP){
      RETURN_NOK_ON_ERR(infoP->biSize != sizeof(BITMAPINFOHEADER),"basisChecks: wrong biSize.");
      RETURN_NOK_ON_ERR(infoP->biPlanes != 1, "basisChecks: wrong biPlanes (must be 1).");
      RETURN_NOK_ON_ERR(infoP->biHeight < 1, "basisChecks: biHeight must be positive.");
      RETURN_NOK_ON_ERR(infoP->biWidth < 1, "basisChecks: biWidth must be positive.");
      RETURN_NOK_ON_ERR((8 != infoP->biBitCount) && (24 != infoP->biBitCount), "basisChecks:  wrong biBitCount (must be 8 or 24).");
      RETURN_NOK_ON_ERR((24 == infoP->biBitCount) && (BI_RGB != infoP->biCompression), "basisChecks: 24 bit format cannot be compressed.");
      RETURN_NOK_ON_ERR((BI_RLE8 != infoP->biCompression) && (BI_RGB != infoP->biCompression), "basisChecks: unexpected compression format.");
      RETURN_NOK_ON_ERR(MAX_COLOR_TABLE_SIZE < infoP->biClrUsed, "basisChecks: biClrUsed out of range.\n");
      RETURN_NOK_ON_ERR((24 == infoP->biBitCount) && (infoP->biClrUsed != 0), "basisChecks: biClrUsed must be 0 for 24 bit/pixel format.");
   }
   return EOK;
}

int readHeaders(void){
   validHeaders = false;
   RETURN_NOK_ON_ERR(1!= COMread((char *) &fileHeader, sizeof(BITMAPFILEHEADER), 1),"readHeaders: Error during read.");
   RETURN_NOK_ON_ERR(1!= COMread((char *) &infoHeader, sizeof(BITMAPINFOHEADER), 1),"readHeaders: Error during read.");
   int erg = basicChecks(&fileHeader, &infoHeader);
   validHeaders = EOK == erg;
   return erg;
}

int getFileHeader(BITMAPFILEHEADER *h){
   *h = fileHeader;
   return ERR_HANDLER(!validHeaders,"getFileHeader: Invalid header.");
}

int getInfoHeader(BITMAPINFOHEADER *h){
   *h = infoHeader;
   return ERR_HANDLER(!validHeaders,"getInfoHeader: Invalid header.");
}

// EOF
