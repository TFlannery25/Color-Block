#ifndef LCDHelper
#define LCDHelper
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdlib.h>

void pulseEnable() {
  PORTC &= ~(1 << PC3);
  _delay_us(1);
  PORTC |= (1 << PC3);
  _delay_us(1);
  PORTC &= ~(1 << PC3);
  _delay_us(100);
}

void write4bits(uint8_t value) {
  if (value & 0x01) PORTD |= (1 << PD4);
  else PORTD &= ~(1 << PD4);

  // Set D5 (PD5)
  if (value & 0x02) PORTD |= (1 << PD5);
  else PORTD &= ~(1 << PD5);

  // Set D6 (PD2) — now moved from PD6
  if (value & 0x04) PORTD |= (1 << PD3);
  else PORTD &= ~(1 << PD3);

  // Set D7 (PD7)
  if (value & 0x08) PORTD |= (1 << PD7);
  else PORTD &= ~(1 << PD7);
  pulseEnable();
}

void send(uint8_t value, uint8_t mode) {
  if (mode)
    PORTD |= (1 << PD2);
  else
    PORTD &= ~(1 << PD2);
  
  write4bits(value >> 4);
  write4bits(value);
}

void lcdCommand(uint8_t cmd) {
  send(cmd, 0);
}

void lcdWrite(uint8_t data) {
  send(data, 1);
}

void lcd_Init() {
  _delay_ms(50);

  write4bits(0x03); 
  _delay_ms(5);
  write4bits(0x03); 
  _delay_us(150);
  write4bits(0x03); 
  _delay_us(150);
  write4bits(0x02);

  lcdCommand(0x28);
  lcdCommand(0x0C);
  lcdCommand(0x01);
  _delay_ms(2);
  lcdCommand(0x06);
}

//Sets the location of the print
void lcdSetCursor(uint8_t col, uint8_t row) {
  const uint8_t row_offsets[] = {0x00, 0x40};
  lcdCommand(0x80 | (col + row_offsets[row]));
}

//Print a passed in unsigned char
void lcdPrint(unsigned char val) {
    char buffer[4]; 
    itoa(val, buffer, 10); 
    char* str = buffer;
    while (*str) 
    {
        lcdWrite(*str++);
    }
}

//Prints a string (pass in "")
void lcdPrintString(const char* str) {
  while (*str) {
    lcdWrite(*str++);
  }
}

//Creates custom character using the char map
void lcdCreateChar(uint8_t location, uint8_t charmap[]) {
  location &= 0x07; 
  lcdCommand(0x40 | (location << 3)); 
  for (int i = 0; i < 8; i++) {
    lcdWrite(charmap[i]);
  }
}

#endif

