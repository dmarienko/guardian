#include <avr/io.h>
#include <util/crc16.h>
#include <util/delay.h>
#include "utils.h"
#include "usart.h"

extern void display_init(void);
extern void display(uint16_t);
extern void display_off(void);
extern void display_err(uint8_t error);

#define HEADER 0x55
#define MAX_PACKET_LEN 128

#define TRANSMITTER

#ifdef TRANSMITTER

static uint8_t tbuff[MAX_PACKET_LEN];

void transmit_packet(uint8_t address, uint8_t* buffer, uint16_t len) {
  int i;
  uint16_t crc = 0xffff;

  // try to wakeup receiver
  usart_transmit(0xff);
  _delay_ms(10);
  usart_transmit(0xff);
  _delay_ms(10);
  usart_transmit(0xff);
  _delay_ms(10);

  // transmit header and address
  usart_transmit(HEADER);   // header
  usart_transmit(address);  // addr
  _delay_ms(50);

  tbuff[0] = (uint8_t) (len >> 8);      // len h
  tbuff[1] = (uint8_t) (len & 0x00ff);  // len l
  for(i=0;i<len;i++) {
	tbuff[i+2] = buffer[i];
  }

  // calc crc
  for(i=0; i<(len+2); i++) 
	crc = _crc16_update(crc, tbuff[i]);
  
  // transmit buffer
  for(i=0; i<(len+2); i++) {
	usart_transmit(tbuff[i]);
  }

  // finally transmit crc
  usart_transmit((uint8_t) (crc >> 8));
  usart_transmit((uint8_t) (crc & 0x00ff));
}

void _transmitter_main(uint8_t address) {
  int i;
  uint8_t buffer[1] = { 0x00 };
  while(1) {
	for(i=0; i<100; i++) {
	  buffer[0] = (uint8_t) i;
	  transmit_packet(address, &buffer, 1);
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
}

int receive_packet(uint8_t address, uint8_t* buff) {
  int i;
  uint16_t crc, len;
  uint8_t b0, crc_h, crc_l, len_h, len_l;
  int r = -1;

  b0 = usart_receive();
  if (b0==HEADER) { // header
	b0 = usart_receive();
	if (b0==address) {   // addr
	  // get packet length
	  len_h = usart_receive();
	  len_l = usart_receive();
	  len = (len_h << 8) | len_l;

	  // wrong length
	  if (len > MAX_PACKET_LEN) return -4;

	  // start receiving buffer
	  for(i=0; i < len; i++) {
		b0 = usart_receive();
		buff[i] = b0;
	  }
	  
	  // receiving CRC16
	  crc_h = usart_receive();
	  crc_l = usart_receive();
	  
	  // calculate crc
	  crc = 0xffff;
	  crc = _crc16_update(crc, len_h);
	  crc = _crc16_update(crc, len_l);
	  for(i=0; i < len; i++)
		crc = _crc16_update(crc, buff[i]);
	  
	  // check crc
	  if (((crc_h << 8) | crc_l)==crc)
		r = len;
	  else r = -3;
	}
  }
  return r;
}

void _receiver_main(uint8_t address) {
  uint8_t buffer[1] = { 0 };
  int i;
  display_init();
  greet();
  display_off();

  while(1) {
	uint16_t r = receive_packet(address, &buffer);
	if(r==-3) display_err(3);
	if(r>0) display(buffer[0]);
  }
}
#endif

int main(void) {
  usart_init(SPEED_4800);

#ifdef TRANSMITTER
  _transmitter_main(1);
#else
  _receiver_main(1);
#endif
    
  return 1;
}
