#include "timer.h"
#include <avr/interrupt.h>
#include <util/atomic.h>

volatile unsigned long timer1_millis;

ISR(TIMER1_COMPA_vect) {
    timer1_millis++;
}

void init_millis(unsigned long f_cpu) {
    unsigned long ctc_match_overflow;

    ctc_match_overflow = ((f_cpu / 1000) / 8); // Calculate the value needed for the CTC match overflow.

    // Initialize counter value to 0.
    TCNT1 = 0;

    // Set CTC match value for 1ms overflow.
    OCR1A = (unsigned int)(ctc_match_overflow);

    // Turn on CTC mode.
    TCCR1B |= (1 << WGM12);

    // Enable timer compare interrupt.
    TIMSK1 |= (1 << OCIE1A);

    // Set CS11 bit for 8 prescaler.
    TCCR1B |= (1 << CS11);

    // Enable global interrupts.
    sei();

    // Initialize the millis counter.
    timer1_millis = 0;
}

unsigned long millis(void) {
    unsigned long millis_return;

    // Ensure this cannot be disrupted.
    ATOMIC_BLOCK(ATOMIC_FORCEON) {
        millis_return = timer1_millis;
    }

    return millis_return;
}
        