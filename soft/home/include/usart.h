#ifndef __USART_H_
#define __USART_H_

#include <avr/io.h>
#include "utils.h"

extern void usart_init(uint16_t);
extern uint8_t usart_receive(void);
extern void usart_transmit(uint8_t);

//#define _SPEED(f)    (F_CPU/(f*16) - 1)
//#define SPEED_9600   _SPEED(9600)
#define SPEED_9600   6 // for 1MHz
#define SPEED_4800   12 // for 1MHz

#endif
