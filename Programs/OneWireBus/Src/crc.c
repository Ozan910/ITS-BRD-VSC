 /*
  *******************************************************************
  * @file crc.c
	*
  * @author Franz Korf, HAW Hamburg 
  * @date April 2016
  * @brief Dieses Modul implementiert den CRC check.	
  *******************************************************************
  */

#include "crc.h"

bool checkCRC(const unsigned int size, const unsigned char arr[size]){
   unsigned char crc = 0x00;
   for (int byteCount = 0; byteCount < size; byteCount ++) {
      unsigned char currentByte = arr[byteCount]; // leftmost bit : first stage, rightmost bit : 8TH stage
      for (int bitCount = 0 ; bitCount < 8 ; bitCount ++) {
         unsigned char intputXorStage8 = (currentByte ^ crc) & 0x01;
         unsigned char newStage0 = (intputXorStage8 << 7);
         unsigned char newStage5 = ((intputXorStage8 << 3) ^ (crc >> 1)) & (0x01 << 3);
         unsigned char newStage6 = ((intputXorStage8 << 2) ^ (crc >> 1)) & (0x01 << 2);
         crc = (crc >> 1) & ~((0x01 << 2) | (0x01 << 3)  | (0x01 << 7));
         crc = crc | newStage0 | newStage5 | newStage6;
         currentByte = currentByte >> 1;
      }
   }
	return 0x00 == crc;
}

bool checkROMCRC(ROM_Number *rom){
    unsigned char arr[8];
    arr[0] = rom->family;
    for(int i = 0; i < 6; i++){
        arr[1+i] = rom->serial[i];
    }
    arr[7] = rom->crc;

    return checkCRC(8, arr);
}

bool checkScratchCRC(Scratchpad *scratchpad){
   unsigned char arr[9];
   uint16_t temp = scratchpad->temperature;
   uint8_t tempLSB = (uint8_t)(temp & 0xFF);
   uint8_t tempMSB = (uint8_t)((temp >> 8) & 0xFF);

   arr[0] = tempLSB;
   arr[1] = tempMSB;

   arr[2] = scratchpad->tHRegister;
   arr[3] = scratchpad->tLRegister;
   arr[4] = scratchpad->configRegister;
   arr[5] = scratchpad->reserved1;
   arr[6] = scratchpad->reserved2;
   arr[7] = scratchpad->reserved3;
   arr[8] = scratchpad->crc;

   return checkCRC(9, arr);
}
// EOF
