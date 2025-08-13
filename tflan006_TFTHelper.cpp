#ifndef TFTHelper
#define TFTHelper

#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>


void SPI_init() {
  SPCR = (1 << SPE) | (1 << MSTR);
}

void SPI_write(uint8_t data) {
  SPDR = data;
  while (!(SPSR & (1 << SPIF))); 
}

void sendCommand(uint8_t cmd) {
  PORTB &= ~(1 << PB0);    
  PORTB &= ~(1 << PB2);    
  SPI_write(cmd);
  PORTB |= (1 << PB2);     
}

void sendData(uint8_t data) {
  PORTB |= (1 << PB0);     
  PORTB &= ~(1 << PB2);    
  SPI_write(data);
  PORTB |= (1 << PB2);     
}

void ST7735_reset() {
  PORTB |= (1 << PB1);
  _delay_ms(100);
  PORTB &= ~(1 << PB1);
  _delay_ms(100);
  PORTB |= (1 << PB1);
  _delay_ms(100);
}

void ST7735_init() {
  ST7735_reset();

  sendCommand(0x01); _delay_ms(150);     
  sendCommand(0x11); _delay_ms(150);     

  sendCommand(0x3A); sendData(0x05); 

  sendCommand(0x29);                
}

void setAddrWindow(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1) {
  sendCommand(0x2A); 
  sendData(0x00); sendData(x0);
  sendData(0x00); sendData(x1);
  sendCommand(0x2B); 
  sendData(0x00); sendData(y0);
  sendData(0x00); sendData(y1);
  sendCommand(0x2C); 
}

void TFTRectangle(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint16_t color) {
  setAddrWindow(x, y, x + w - 1, y + h - 1);
  PORTB |= (1 << PB0);
  PORTB &= ~(1 << PB2);
  for (uint16_t i = 0; i < w * h; i++) {
    SPI_write(color >> 8);
    SPI_write(color & 0xFF);
  }
  PORTB |= (1 << PB2);
}

void TFT_Init() {
  SPI_init();
  ST7735_init();
  TFTRectangle(0, 0, 130, 130, 0x0000);
}

#endif

