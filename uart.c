/**
*
* File: uart.c
* Project: JVC remote control for Prius NHW20
* Author: Eduard Haag
* Description: Software UART at 9600Kbps
*
* History:
*
* 05/04/2017 initial version
* 14/04/2017 stable version
* 21/09/2017 comment refactoring
*
**/

#include "config.h"
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include "uart.h"

// uart tx pin high
void uart_tx_pin_high() {
  TX_PORTX |= (1 << TX_PIN);
}

// uart tx pin low
void uart_tx_pin_low() {
  TX_PORTX &= ~(1 << TX_PIN);
}

// send uart
void send_uart(const uint8_t c) {
  uint8_t t = c;
  uart_tx_pin_low();
  _delay_us(SW_UART_DELAY);
  for(uint8_t i = 0; i < 8; i++) {
    if (t & 0x01) {
      uart_tx_pin_high();
    } else {
      uart_tx_pin_low();
    }
    _delay_us(SW_UART_DELAY);
    t >>= 1;
  }
  uart_tx_pin_high();
  _delay_us(SW_UART_DELAY);
}

// send uart str
void send_uart_str(const char *s) {
  while (*s != 0) {
    send_uart((uint8_t)*s++);
  }
}

// send uart str from flash
void send_uart_pstr(const char *pstr) {
  while (pgm_read_byte(pstr) != '\0') {
    send_uart(pgm_read_byte(pstr));
    pstr++;
  }
}

// init uart
void init_uart() {
  TX_DDRX |= (1 << TX_PIN);
  TX_PORTX |= (1 << TX_PIN);
}
