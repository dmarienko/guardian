#ifndef __USART_H_
#define __USART_H_

#include <avr/io.h>
#include "utils.h"

extern void usart_init(unsigned int);
extern unsigned char usart_receive(void);
extern void usart_transmit(unsigned char);

//#define _SPEED(f)    (F_CPU/(f*16) - 1)
//#define SPEED_9600   _SPEED(9600)
#define SPEED_9600   0x6 // for 1MHz

#endif
