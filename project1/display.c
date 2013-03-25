#include <avr/io.h>
#include <util/delay.h>
#include "utils.h"

#define DATA_BUS (B3|B2|B1|B0)
#define LATCH_BIT B4
#define DATA_PORT PORTC

static void write_data_to_bus(unsigned char c) {
  unsigned char ch = (c & 0xf0) >> 4;
  unsigned char cl = (c & 0x0f);
  /* --- first version ---
	PORTB &= 0xE1;
	PORTB |= _BV(PB5);
	PORTB |= cl;
	PORTB &= ~_BV(PB5);
	PORTB &= 0xE1;
	PORTB |= ch;
	PORTB |= _BV(PB5);
   */

  // clear data bus from previous value
  bit_clear(DATA_PORT, DATA_BUS);

  // arm control bit
  bit_set(DATA_PORT, LATCH_BIT);
  _delay_us(1);

  // write low part to databus
  bit_set(DATA_PORT, cl);

  // trigger control bit - it latches low data
  bit_clear(DATA_PORT, LATCH_BIT);
  _delay_us(1);

  // clear data bus from previous value
  bit_clear(DATA_PORT, DATA_BUS);

  // write high part to databus
  bit_set(DATA_PORT, ch);
  
  // trigger control bit - it latches high data
  bit_set(DATA_PORT, LATCH_BIT);
}


void display(unsigned short v) {
  unsigned short l = v % 10;
  write_data_to_bus((((v-l)/10)<<4) | l);
}

void display_off() {
  write_data_to_bus(0xff);
}