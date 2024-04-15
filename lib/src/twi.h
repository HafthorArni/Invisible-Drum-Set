#ifndef TWI_H
#define TWI_H

#include <stdint.h> // For uint8_t type

// TWI status codes
#define TW_START                0x08  // start condition transmitted
#define TW_REP_START            0x10  // repeated start condition transmitted
#define TW_MT_SLA_ACK           0x18  // SLA+W transmitted, ACK received
#define TW_MT_SLA_NACK          0x20  // SLA+W transmitted, NACK received
#define TW_MT_DATA_ACK          0x28  // data transmitted, ACK received
#define TW_MT_DATA_NACK         0x30  // data transmitted, NACK received
#define TW_MR_SLA_ACK           0x40  // SLA+R transmitted, ACK received
#define TW_MR_SLA_NACK          0x48  // SLA+R transmitted, NACK received
#define TW_MR_DATA_ACK          0x50  // data received, ACK returned
#define TW_MR_DATA_NACK         0x58  // data received, NACK returned


// Function declarations for TWI (I2C) operations
void TWI_init(void);
void TWI_start(void);
void TWI_stop(void);
void TWI_write(uint8_t data);
uint8_t TWI_readACK(void);
uint8_t TWI_readNACK(void);
void twi_write_byte(uint8_t addr, uint8_t reg, uint8_t val);
uint8_t twi_read_byte(uint8_t addr, uint8_t reg);
void twi_read_buffer(uint8_t addr, uint8_t reg, uint8_t* buffer, uint8_t len);

#endif // TWI_H
