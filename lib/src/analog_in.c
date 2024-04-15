#include "analog_in.h"

void adc_init() {
    // Set the reference voltage to AVcc (5V)
    ADMUX |= (1 << REFS0);
    // Enable the ADC and set the prescaler to 128 (for 16MHz clock)
    // This results in a 125kHz ADC clock, which is within the 50-200kHz recommended range.
    ADCSRA |= (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
}

uint16_t adc_read(uint8_t channel) {
    // Select the corresponding channel 0~7
    // ANDing with '7' will always keep the value
    // of 'channel' between 0 and 7
    channel &= 0b00000111;  // AND operation with 7
    ADMUX = (ADMUX & 0xF8)|channel; // clears the bottom 3 bits before ORing

    // Start single conversion
    // Write '1' to ADSC
    ADCSRA |= (1 << ADSC);

    // Wait for conversion to complete
    // ADSC becomes '0' again
    // till then, run loop continuously
    while(ADCSRA & (1 << ADSC));

    return (ADC);
}
