#ifndef U_GAMEPAD_BITMAPS_H
#define U_GAMEPAD_BITMAPS_H

#ifdef NATIVE
#define PROGMEM
#else
#include <avr/pgmspace.h>
#endif

extern unsigned char bmp_gamepad_64x43 PROGMEM[];

extern unsigned char bmp_gamepad_90x60 PROGMEM[];

#endif //U_GAMEPAD_BITMAPS_H