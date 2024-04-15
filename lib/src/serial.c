#include <avr/io.h>
#include "serial.h"
#include <stdio.h> 
#include <stdlib.h> 
#include "serial.h"

void SerialBegin(uint32_t baudrate) {
    uint16_t baud_prescale;
    float error_rate;

    // Enable double speed if using high baud rates or if it reduces error rate
    error_rate = (F_CPU / (double)baudrate / 8.0 - 1) - (int)(F_CPU / (double)baudrate / 8.0 - 1);
    if (baudrate > 57600 || error_rate < 0.02) { // Adjust the error_rate threshold as needed
        // Enable double speed
        UCSR0A = (1 << U2X0);
        baud_prescale = (uint16_t)((F_CPU / (double)baudrate / 8.0) - 1);
    } else {
        // Disable double speed
        UCSR0A &= ~(1 << U2X0);
        baud_prescale = (uint16_t)((F_CPU / (double)baudrate / 16.0) - 1);
    }

    // Set baud rate
    UBRR0H = (baud_prescale >> 8);
    UBRR0L = baud_prescale;

    // Enable receiver and transmitter
    UCSR0B = (1 << RXEN0) | (1 << TXEN0);

    // Set frame format: 8 data bits, no parity, 1 stop bit
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}


void SerialPrintChar(char c) {
    // Wait for empty transmit buffer
    while (!(UCSR0A & (1 << UDRE0)));

    // Put data into buffer, sends the data
    UDR0 = c;
}

void SerialPrintStr(const char *str) {
    while (*str) {
        SerialPrintChar(*str++);
    }
}

void SerialPrintInt(int16_t num) {
    char buf[7]; // Buffer to hold the converted integer plus a sign and null terminator
    sprintf(buf, "%d", num); // Convert the integer to a string
    SerialPrintStr(buf); // Print the string
}

void SerialPrintFloat(float number) {
    // Convert the integer part of the number
    int intPart = (int)number;
    // Calculate the fractional part, with a fixed number of digits of precision
    float fractionalPart = number - (float)intPart;
    // Adjust for precision and rounding
    int fractionalAsInt = (int)(fractionalPart * 10000.0 + 0.5); // 4 digits of precision

    char buf[20]; // Ensure this buffer is large enough for the largest number you plan to print

    // Handle negative numbers
    if (number < 0.0) {
        fractionalAsInt = -fractionalAsInt; // Make fractional part positive for negative numbers
    }
    // Create a string from the integer and fractional parts
    sprintf(buf, "%d.%04d", intPart, fractionalAsInt); // Adjust "%04d" based on desired precision
    SerialPrintStr(buf); // Use existing function to print the string
}
void SerialPrintByte_as_binary(uint8_t byte) {
    for (int i = 7; i >= 0; i--) {
        if (byte & (1 << i)) {
            SerialPrintChar('1');
        } else {
            SerialPrintChar('0');
        }
    }
}

void SerialPrintByte_as_hex(uint8_t byte) {
    char buf[3]; // Buffer to hold the converted byte plus null terminator
    sprintf(buf, "%02X", byte); // Convert the byte to a two-digit hexadecimal string
    SerialPrintStr(buf); // Print the string
}