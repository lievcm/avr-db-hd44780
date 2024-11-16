/*
* Author: Liev Mitchell
* Date: Feb-19-2024
* License: CC0 1.0 (https://creativecommons.org/publicdomain/zero/1.0/)
* Description: A very basic C library for interfacing between DB series AVR MCUs and 16x2 character type LCDs using
*              the Hitachi HD44780 LCD controller (or equivalent). uses a PCF8574 IO expander connected to
*              pins A2 & A3
*/

#ifndef LCDLIB_H
#define LCDLIB_H

#include <avr/io.h>

#define LCD_ADDR 0b0111000          // Bus expander Address


/**
 * Writes the given 8 bits of data to the command register of the LCD
 * 
 * @param cmd 8-bit command to be written
 */
void lcd_send_command(uint8_t cmd);


/*
* Writes the given 8 bits of data to the data register of the LCD
*
* Args:
*      ch: 8-bit character code to be written to the LCD 
*          (Use table 4 for A02 ROM in datasheet, mostly ascii compatible)
*/
void lcd_send_char(uint8_t ch);


/*
* Initializes the LCD based on the given display parameters. Automatically
* configures the display as two line with 5x8 font, and sets the cursor to
* increment on write
*
* Args:
*     cursor: 0 to disable cursor appearance, 1 to enable
*     blink: 0 to disable cursor blinking, 1 to enable
*     display: 0 to turn display off, 1 to turn on
*/
void lcd_init(uint8_t cursor, uint8_t blink, uint8_t display);


/*
* Sets the cursor location (where data is written) based on given row & offset
*
* Args:
*     row: int from 0 to 1 corresponding to the row
*     offset: int from 0 to 15 corresponding to the column 
*/
void lcd_set_cursor_xy(uint8_t row, uint8_t offset);


/*
* Enables or disables cursor blinking
*
* Args:
*     en: 0 to disable blinking, 1 to enable
*/
void lcd_set_blink_en(uint8_t en);


/*
* Enables or disables the cursor's appearance
*
* Args:
*     en: 0 to disable cursor, 1 to enable
*/
void lcd_set_cursor_en(uint8_t en);


/*
* Turns display on or off
*
* Args:
*     en: 0 to turn off display, 1 to turn on
*/
void lcd_set_display_en(uint8_t en);


/*
* Writes a string to the display at the current cursor location
*
* Args:
*     s: points to the beginning of the string to be written
*/
void lcd_send_string(const char *s);


/*
* Writes a string to the display starting at the given location
*
* Args:
*     s: points to the beginning of the display to be written
*     row: int from 0 to 1 corresponding to the row
*     offset: int from 0 to 15 corresponding to the column
*/
void lcd_send_string_xy(const char *s, uint8_t row, uint8_t offset);


/*
* Clears the display, sets the cursor to the home position
*/
void lcd_clear(void);

/**
 * Clears 1 row of the display
 * @param row: int from 0 to 1 corresponding to the row to be cleared
 */
void lcd_clear_row(uint8_t row);

#endif