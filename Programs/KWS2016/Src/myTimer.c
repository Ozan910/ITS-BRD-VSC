#include "myTimer.h"
#include "timer.h"

void mySleep(uint32_t t){
    uint32_t start = getTimeStamp();
    
    while((getTimeStamp() - start) < t){
        //ZzZzZz
    }
}