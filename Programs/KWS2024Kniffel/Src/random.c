/**
 ******************************************************************************
 * @file    random.c
 * @author  Franz Korf
 *          HAW-Hamburg
 *          Labor fuer technische Informatik
 *          Berliner Tor  7
 *          D-20099 Hamburg
 * @version 1.0
 *
 * @date    20. Jan. 2025
 * @brief   Dieses Modul kapselt den Standard-Pseudozufallsgenerator fuer die
 *          Kniffel Klausuraufgabe.
 ******************************************************************************
 */
 
#include "random.h"
#include <stdlib.h>
#include <stdbool.h>

int nxtRandNumber(void){
   const unsigned int seed = 1302;
   static bool firstCall = true;
   if (firstCall) {
      firstCall = false;
      srand(seed);
   }
   int erg = rand();
   return erg;
}
// EOF