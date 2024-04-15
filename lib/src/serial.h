#ifndef SERIAL_H
#define SERIAL_H

#include <avr/io.h>
#include "serial.h"
#include <stdio.h> // For sprintf
#include <stdlib.h> 

#ifndef F_CPU
#define F_CPU 16000000UL
#endif
#define BAUD_PRESCALE(baud) (((F_CPU / (baud * 16UL))) - 1)
void SerialBegin(uint32_t baudrate);
void SerialPrintChar(char c);
void SerialPrintStr(const char *str);
void SerialPrintInt(int16_t num);
void SerialPrintFloat(float number);
void SerialPrintByte_as_binary(uint8_t byte);
void SerialPrintByte_as_hex(uint8_t byte);

#endif
