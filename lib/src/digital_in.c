#include "digital_in.h"

void digital_in_init(uint8_t pin) {
    // Set the pin as input
    DDRD &= ~(1 << pin);
    // Enable pull-up resistor
    PORTD |= (1 << pin);
}

uint8_t digital_in_read(uint8_t pin) {
    // Read and return the pin state (0 or 1)
    return (PIND & (1 << pin)) == 0; // Active low
}
