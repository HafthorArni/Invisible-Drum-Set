#include "twi.h"
#include <avr/io.h>

#define F_SCL 400000UL // SCL frequency
#define Prescaler 1
#define TWBR_value (((F_CPU / F_SCL) - 16) / (2 * Prescaler))

void TWI_init(void) {
    TWSR = 0x00; // set prescale to 1
    TWBR = (uint8_t)TWBR_value;  // set SCL frequency
    TWCR = (1<<TWEN); // enable TWI
}

void TWI_start(void) {
    TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
    while (!(TWCR & (1<<TWINT)));
}

void TWI_stop(void) {
    TWCR = (1<<TWINT)|(1<<TWSTO)|(1<<TWEN);
    while(TWCR & (1<<TWSTO));  // Wait for stop condition to be executed on bus
}

void TWI_write(uint8_t data) {
    TWDR = data; // Load data into TWI data register
    TWCR = (1<<TWINT)|(1<<TWEN); // Start TWI and clear interrupt flag
    while (!(TWCR & (1<<TWINT))); // Wait until TWI finish its current job
}

uint8_t TWI_readACK(void) {
    TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWEA);
    while (!(TWCR & (1<<TWINT)));
    return TWDR;
}

uint8_t TWI_readNACK(void) {
    TWCR = (1<<TWINT)|(1<<TWEN);
    while (!(TWCR & (1<<TWINT)));
    return TWDR;
}

void twi_write_byte(uint8_t addr, uint8_t reg, uint8_t val) {
    TWI_start();
    TWI_write(addr << 1);
    TWI_write(reg);
    TWI_write(val);
    TWI_stop();
}

uint8_t twi_read_byte(uint8_t addr, uint8_t reg) { 
    TWI_start(); // Start the I2C write transaction
    TWI_write(addr << 1); // Send the slave address with the write bit
    TWI_write(reg); // Send the register address
    TWI_start();  // Send a repeated start for the read transaction
    TWI_write((addr << 1) | 0x01); // Send the slave address with the read bit   
    uint8_t val = TWI_readNACK();  // Read the data byte without sending an ACK
    TWI_stop(); // Stop the I2C transaction
    return val; // Return the read data byte
}


void twi_read_buffer(uint8_t addr, uint8_t reg, uint8_t* buffer, uint8_t len) {
    TWI_start();
    TWI_write(addr << 1);
    TWI_write(reg);
    TWI_start();
    TWI_write((addr << 1) | 0x01);
    for(int i = 0; i < len - 1; i++) {
        buffer[i] = TWI_readACK();
    }
    buffer[len - 1] = TWI_readNACK();
    TWI_stop();
}