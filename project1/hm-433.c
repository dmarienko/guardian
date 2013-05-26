#include "hm-433.h"
#include "usart.h"

static uint8_t tbuff[MAX_PACKET_LEN];

void hm433_init(uint16_t speed) {
    usart_init(speed);
}

/**
 * Try to transmitt datagram to HM-R433 module
 */
void hm433_transmit_packet(uint8_t address, uint8_t* buffer, uint16_t len) {
    int i;
    uint16_t crc = 0xffff;

    // try to wakeup receiver
    usart_transmit(0xff);
    usart_transmit(0xff);
    usart_transmit(0xff);

    // transmit header and address
    usart_transmit(HEADER); // header
    usart_transmit(address); // addr

    tbuff[0] = (uint8_t) (len >> 8); // len h
    tbuff[1] = (uint8_t) (len & 0x00ff); // len l
    for (i = 0; i < len; i++) {
        tbuff[i + 2] = buffer[i];
    }

    // calc crc
    for (i = 0; i < (len + 2); i++)
        crc = _crc16_update(crc, tbuff[i]);

    // transmit buffer
    for (i = 0; i < (len + 2); i++) {
        usart_transmit(tbuff[i]);
    }

    // finally transmit crc
    usart_transmit((uint8_t) (crc >> 8));
    usart_transmit((uint8_t) (crc & 0x00ff));
}

/**
 * Try to receive datagram from HM-R433 module
 */
int hm433_receive_packet(uint8_t address, uint8_t* buff) {
    int i;
    uint16_t crc, len;
    uint8_t b0, crc_h, crc_l, len_h, len_l;
    int r = -1;

    b0 = usart_receive();
    if (b0 == HEADER) { // header
        b0 = usart_receive();
        if (b0 == address) { // addr
            // get packet length
            len_h = usart_receive();
            len_l = usart_receive();
            len = (len_h << 8) | len_l;

            // wrong length
            if (len > MAX_PACKET_LEN) return -4;

            // start receiving buffer
            for (i = 0; i < len; i++) {
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
            for (i = 0; i < len; i++)
                crc = _crc16_update(crc, buff[i]);

            // check crc
            if (((crc_h << 8) | crc_l) == crc)
                r = len;
            else r = -3;
        }
    }
    return r;
}
