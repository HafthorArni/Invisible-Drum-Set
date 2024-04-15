#ifndef TIME_H
#define TIME_H

#include <avr/io.h>

// Initializes Timer 1 to count milliseconds
void init_millis(unsigned long f_cpu);

// Returns the number of milliseconds since the program started
unsigned long millis(void);

#endif // TIME_H
