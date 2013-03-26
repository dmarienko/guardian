#include "usart.h"

void usart_init(unsigned int ubrr) {
  /* Set baud rate */
  UBRRH = (unsigned char) (ubrr>>8); // set speed
  UBRRL = (unsigned char) ubrr;
  UCSRB = BIT(RXEN) | BIT(TXEN);     // tx/dx is enabled
  UCSRC = 0x86;
  UCSRC = BIT(URSEL) | BIT(USBS) | (3<<UCSZ0);
}

unsigned char usart_receive(void) {
  while (!bit_get(UCSRA, BIT(RXC)));
  return UDR; 
}

void usart_transmit(unsigned char data) {
  while (!bit_get(UCSRA, BIT(UDRE)));
  UDR = data;
}
