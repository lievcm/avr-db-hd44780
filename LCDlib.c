#define F_CPU 4000000UL

#include "LCDlib.h"
#include "I2C.h"
#include <util/delay.h>

#define EN 0b00000100
#define RS 0b00001000
#define CLEAR 0x01
#define SET_DDR_ADDR 0x80

//Sends the least significant nibble (4 bits) of input nib
void send_nibble(uint8_t nib);

static uint8_t display_params = 0x08;
static uint8_t io_xpandr_port = 0b00000000;

void lcd_send_command(uint8_t cmd) {
    io_xpandr_port &= ~RS;
    send_nibble(cmd >> 4);                      //Send upper nibble, command reg
    send_nibble(cmd);                           //Send lower nibble
    _delay_us(150);
}

void lcd_send_char(uint8_t ch) {
    io_xpandr_port |= RS;
    send_nibble(ch >> 4);                       //Send upper nibble, data reg
    send_nibble(ch);                            //Send lower nibble
    _delay_us(150);
}

void send_nibble(uint8_t nib) {
    io_xpandr_port = (nib << 4) | (io_xpandr_port & 0x0F);
    io_xpandr_port |= EN;
    I2C_start_transfer(LCD_ADDR, 0);
    I2C_send_byte(io_xpandr_port, 0);
    io_xpandr_port &= ~EN;
    _delay_us(5);
    I2C_send_byte(io_xpandr_port, 1);
    _delay_us(5);
}

void lcd_init(uint8_t cursor, uint8_t blink, uint8_t display) {

    _delay_ms(20);                              //Wait minimum 15ms

    send_nibble(0x3);                           //Initialization as per datasheet
    _delay_ms(4.1);                             //min 4.1ms
    send_nibble(0x3);
    _delay_us(100);                             //min 100us
    send_nibble(0x3);

    send_nibble(0x2);                           //Initialize 4 bit mode
    lcd_send_command(0x28);                         //4 bit mode, 2 line, small font

    display_params |= (display << 2)            //Set display parameters
                    | (cursor << 1) 
                    | blink;

    lcd_send_command(display_params);               //Send display parameters

    lcd_send_command(CLEAR);                        //Clear the display
    _delay_ms(2.5);

    lcd_send_command(0x06);                         //Set cursor to increment on write
}

void lcd_set_cursor_xy(uint8_t row, uint8_t offset) {
    uint8_t addr;
    if(row == 0) {
        addr = 0x00;                            //Row 1 starts @ DDRAM address 0x00
    }
    else {
        addr = 0x40;                            //Row 2 starts @ DDRAM address 0x40
    }

    addr += offset;                             //Add column offset

    lcd_send_command(SET_DDR_ADDR | addr);          //Set cursor address
}

void lcd_set_blink_en(uint8_t en) {
    if(en) {
        display_params |= 0x01;                 //Set blink on/off bit to 1
    }
    else {
        display_params &= ~0x01;                //Set blink on/off bit to 0
    }

    lcd_send_command(display_params);               //Send updated params
}

void lcd_set_cursor_en(uint8_t en) {
    if(en) {
        display_params |= 0x02;                 //Set cursor on/off bit to 1
    }
    else {
        display_params &= ~0x02;                //Set cursor on/off bit to 0
    }

    lcd_send_command(display_params);               //Send updated params
}

void lcd_set_display_en(uint8_t en) {
    if(en) {
        display_params |= 0x04;                 //Set display on/off bit to 1
    }
    else {
        display_params &= ~0x04;                //Set display on/off bit to 0
    }

    lcd_send_command(display_params);               //Send updated params
}

void lcd_send_string(const char *s) {
    int i;
    for(i = 0; s[i] != 0; i++) {
        lcd_send_char(s[i]);
    }
}

void lcd_send_string_xy(const char *s, uint8_t row, uint8_t offset) {
    lcd_set_cursor_xy(row, offset);
    lcd_send_string(s);
}

void lcd_clear(void) {
    lcd_send_command(CLEAR);
    _delay_ms(1.5);
}

void lcd_clear_row(uint8_t row) {
    lcd_set_cursor_xy(row, 0);
    uint8_t i;
    for(i = 0; i < 16; i++){
        lcd_send_char(' ');
    }
}
