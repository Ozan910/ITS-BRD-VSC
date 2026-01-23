/**
 * @file myerr.c
 * @author Franz Korf, HAW Hamburg 
 * @date Dezember 2016
 * @brief Dieses Modul gehört zur Lösung der GS Probeklausur WS 2016-17. 
 */

#include "myerr.h"

// error speichert den zuletzt gemeldeten Fehler.
static char error = EOK;

void setError(int errno){
	 error = errno;
}

int getError(void){
	 return error;
}

void resetError(void) {
	 error = EOK;
}

// EOF
