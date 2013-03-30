#include <avr/io.h>
#include <util/crc16.h>
#include <util/delay.h>
#include "utils.h"
#include "usart.h"

extern void display_init(void);
extern void display(uint16_t);
extern void display_off(void);
extern void display_err(uint8_t error);

uint8_t tbuff[128];

//#define TRANSMITTER

#ifdef TRANSMITTER

void transmit_packet(uint8_t* buffer, uint16_t len) {
  int i;
  uint16_t crc = 0xffff;

  tbuff[0] = 0x55;  // header
  tbuff[1] = 0x01;  // addr
  tbuff[2] = (uint8_t) (len >> 8);      // len h
  tbuff[3] = (uint8_t) (len & 0x00ff);  // len l
  for(i=0;i<len;i++) {
	tbuff[i+4] = buffer[i];
  }
  
  // start transmission
  for(i=0; i<(len+4); i++) {
	usart_transmit(tbuff[i]);
	crc = _crc16_update(crc, tbuff[i]);
	_delay_ms(10);
  }
  usart_transmit((uint8_t) (crc >> 8));
  _delay_ms(10);
  usart_transmit((uint8_t) (crc & 0x00ff));
}

void _transmitter_main(void) {
  int i;
  uint8_t buffer[1] = { 0x00 };
  while(1) {
	for(i=0; i<100; i++) {
	  buffer[0] = (uint8_t) i;
	  transmit_packet(&buffer, 1);
	  _delay_ms(1000);
	}
  }
}
#else

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

void _receiver_main(void) {
  int i;
  display_init();
  greet();
  display_off();

  while(1) {
	uint16_t crc = 0xffff;
	uint16_t crc_r, len = 0;
	uint8_t b0, b1;

	b0 = usart_receive();
  	if(b0==0x55) { // header
	  crc = _crc16_update(crc, b0);

	  b0 = usart_receive();
	  if (b0==0x01) {   // addr
		crc = _crc16_update(crc, b0);

		// high len
		b0 = usart_receive();
		crc = _crc16_update(crc, b0);
		b1 = usart_receive();
		crc = _crc16_update(crc, b1);
		len = (b0 << 8) | b1;

		for(i=0; i < len; i++) {
		  b0 = usart_receive();
		  tbuff[i] = b0;
		  crc = _crc16_update(crc, b0);
		}
		b0 = usart_receive();
		_delay_ms(10);
		b1 = usart_receive();
		crc_r = (b0 << 8) | b1;
		if (crc_r==crc) { display(tbuff[0]); } else display_err(3);
	  }  else display_err(2);
	} else display_err(1);
  }
}
#endif

int main(void) {
  usart_init(SPEED_4800);

#ifdef TRANSMITTER
  _transmitter_main();
#else
  _receiver_main();
#endif
    
  return 1;
}
