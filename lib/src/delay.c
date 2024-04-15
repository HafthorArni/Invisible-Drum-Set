#include "delay.h"
#include <avr/io.h>


#define CPU_FREQ 16000000UL //16MHz clock
#define CYCLES_PER_US (CPU_FREQ / 1000000UL) // Cycles per microsecond

#define ADJUSTMENT_FACTOR 27 // Adjusted by testing

void Delay_us(unsigned int iMicroseconds) {
    unsigned volatile long cycles = (CYCLES_PER_US * iMicroseconds) / ADJUSTMENT_FACTOR;
    while(cycles--) {
        // Empty loop
    }
}

void Delay_ms(unsigned int iMilliseconds) {
    while(iMilliseconds--) {
        Delay_us(1000);
    }
}

void Delay_s(unsigned int iSeconds) {
    while(iSeconds--) {
        Delay_ms(1000);
    }
}
