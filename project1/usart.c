#include "usart.h"

void usart_init(uint16_t ubrr) {
  /* Set baud rate */
  UBRRH = (uint8_t) (ubrr>>8); // set speed
  UBRRL = (uint8_t) ubrr;
  UCSRB = BIT(RXEN) | BIT(TXEN);     // tx/dx is enabled
  UCSRC = 0x86;
  UCSRC = BIT(URSEL) | BIT(USBS) | (3<<UCSZ0);
}

uint8_t usart_receive(void) {
  while (!bit_get(UCSRA, BIT(RXC)));
  return UDR; 
}

void usart_transmit(uint8_t data) {
  while (!bit_get(UCSRA, BIT(UDRE)));
  UDR = data;
}
