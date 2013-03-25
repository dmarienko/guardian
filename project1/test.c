#include <avr/io.h>
#include <util/delay.h>
#include "utils.h"

extern void display(unsigned short v);
extern void display_off();

void greet() {
  int i;

  bit_set(PORTD, B0);	
  _delay_ms(500);
  bit_clear(PORTD, B0);	

  for (i=0; i<5; i++) {
	display_off();
	_delay_ms(500);
	display(88);
	_delay_ms(500);
  }
}

int main(void) {
  DDRD = 0xff;		// port D as outputs
  DDRC = 0xff;		// port C as outputs
  
  int i, j;

  for (i=0; i<200; i++) {
	greet();

	for(j=0; j<100; j++) {
	  display(j);
	  _delay_ms(500);
	}
  }
  
  return 1;
}
