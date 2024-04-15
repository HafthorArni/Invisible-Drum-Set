#ifndef DIGITAL_IN_H
#define DIGITAL_IN_H

#include <avr/io.h> // Include to access IO registers

// Function prototypes
void digital_in_init(uint8_t pin);
uint8_t digital_in_read(uint8_t pin);

#endif
