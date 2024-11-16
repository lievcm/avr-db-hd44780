#include "I2C.h"
#include <avr/cpufunc.h>

void init_I2C(void) {
    PORTA.PIN2CTRL |= PORT_PULLUPEN_bm;
    PORTA.PIN3CTRL |= PORT_PULLUPEN_bm;
    
    TWI0.MBAUD = (uint8_t)TWI0_BAUD(I2C_SCL_FREQ, 0.1); //Set baud rate
    
    TWI0.MCTRLA = 0b00000001; //Enable as master
    
    TWI0.MSTATUS = 0x01; //Set bus state to idle
}

void I2C_start_transfer(uint8_t addr, uint8_t dir) {
    TWI0.MADDR = (addr << 1) | dir;
    while (TWI0.MSTATUS & 0b00010000) { //Wait for ACK from slave
        ;
    }
}

uint8_t I2C_send_byte(uint8_t data, uint8_t end_trans) {
    while (!(TWI0.MSTATUS & 0b01000000)) {
        ;
    }
    TWI0.MDATA = data; //Write Data
    while (!(TWI0.MSTATUS & 0b01000000)) { //Wait for data to transmit
        ;
    }
    uint8_t ack = (TWI0.MSTATUS & 0b00010000) >> 4; //read ack bit
    
    if(end_trans | ack) { //end transmission or nack bit received
        TWI0.MCTRLB = 0x03; //Send stop condition (with nack?)
    }
    
    return ack; //return ack bit
}

uint8_t I2C_read_byte(uint8_t end_trans) {
    while (!(TWI0.MSTATUS & 0b10000000)) { //Wait for data to be received
        ;
    }
    uint8_t read_data = TWI0.MDATA; //Copy received data
    if (end_trans) {    //End the transmission
        TWI0.MCTRLB = 0x07; //Send stop condition with NACK to end communication
    }
    else {
        TWI0.MCTRLB = 0x02; //Initiate next read
    }
    
    return read_data;
}
