#ifndef IO_H
#define IO_H

#include <stdbool.h>
#include <stdint.h>

void setLEDs(uint8_t noLeds);

bool buttonPressed(uint8_t button);

#endif
//eof