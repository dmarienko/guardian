#include <avr/io.h>
#include <util/delay.h>
#include "utils.h"
#include "usart.h"

extern void display_init(void);
extern void display(unsigned short);
extern void display_off(void);

void greet(void) {
  int i;

  for (i=0; i<3; i++) {
	display_off();
	_delay_ms(500);
	display(88);
	_delay_ms(500);
  }

  /* for(i=0; i<100; i++) { */
  /* 	display(i); */
  /* 	_delay_ms(50); */
  /* } */
  //  _delay_ms(1000);
}

int main(void) {
  int i, j;
  usart_init(SPEED_9600);

#if 1
  display_init();
  greet();
  display_off();

  while(1) {
  	int r = (int) usart_receive();
  	display(r);
  }
#else
  while(1) {
	for(i=0;i<100;i++) {
	  usart_transmit((unsigned char) i);
	  _delay_ms(1000);
	}
  }
#endif
    
  return 1;
}
