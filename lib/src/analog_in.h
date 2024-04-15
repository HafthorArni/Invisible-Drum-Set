#ifndef ANALOGIN_H
#define ANALOGIN_H

#include <avr/io.h>

void adc_init(); // Initializes the ADC
uint16_t adc_read(uint8_t channel); // Reads an analog value from the specified channel

#endif // ANALOGIN_H
