#ifndef __HM_433_H_
#define __HM_433_H_

#include <avr/io.h>
#include <util/crc16.h>

#define HEADER 0x55
#define MAX_PACKET_LEN 128

void hm433_init(uint16_t speed);
void hm433_transmit_packet(uint8_t addr, uint8_t* buff, uint16_t len);
int hm433_receive_packet(uint8_t addr, uint8_t* buff);

#endif
