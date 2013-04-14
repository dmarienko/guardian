#include <avr/io.h>
#include <util/delay.h>
#include "utils.h"
#include "usart.h"
#include "hm-433.h"

extern void display_init(void);
extern void display(uint16_t);
extern void display_off(void);
extern void display_err(uint8_t error);

//#define TRANSMITTER

#ifdef TRANSMITTER

void _transmitter_main(uint8_t address) {
    int i;
    uint8_t buffer[1] = { 0x00 };
    while(1) {
        for(i=0; i<100; i++) {
            buffer[0] = (uint8_t) i;
            hm433_transmit_packet(address, (uint8_t*) &buffer, 1);
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


void _receiver_main(uint8_t address) {
    uint8_t buffer[1] = { 0 };

    display_init();
    greet();
    display_off();

    while(1) {
        uint16_t r = hm433_receive_packet(address, (uint8_t*) &buffer);
        if(r==-3) display_err(3);
        if(r>0) display(buffer[0]);
    }
}
#endif

int main(void) {
    hm433_init(SPEED_4800);

#ifdef TRANSMITTER
    _transmitter_main(1);
#else
    _receiver_main(1);
#endif

    return 1;
}
