#ifndef __UTILS_H_
#define __UTILS_H_

#define BIT(x)  (0x01 << (x)) 
#define B0      0x01
#define B1      0x02
#define B2      0x04
#define B3      0x08
#define B4      0x10
#define B5      0x20
#define B6      0x40
#define B7      0x80
#define bit_get(p,m) ((p) & (m)) 
#define bit_set(p,m) ((p) |= (m)) 
#define bit_clear(p,m) ((p) &= ~(m)) 
#define bit_flip(p,m) ((p) ^= (m)) 
#define bit_write(c,p,m) (c ? bit_set(p,m) : bit_clear(p,m)) 
//#define LONGBIT(x) ((unsigned long)0x00000001 << (x)) 

#endif
