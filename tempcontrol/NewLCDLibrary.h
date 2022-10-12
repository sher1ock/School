// LCD library code for 16x2 LCD connected to Raspberry Pi Pico
// David Frame Mechatronics Utah Valley University 2022
#ifndef NEWLCDLIBRARY
#define NEWLCDLIBRARY
#include <stdio.h>
#include "pico/stdlib.h"

#define BLINK true
#define NO_BLINK false
// Pin positions in LCDpins array
#define RS 4
#define E 5
// Pin values
#define HIGH 1
#define LOW 0
// LCD pin RS meaning
#define COMMAND 0
#define DATA 1

// prototypes
void LCDinit(int bit4_pin, int bit5_pin, int bit6_pin, int bit7_pin, int rs_pin, int e_pin, int width, int depth);
void LCDclear(void);
void LCDprintWrapped(const char *str);
void LCDprint(const char *str);
void LCDgotoPos(int pos_i, int line);
void LCDdisplayOn();
void LCDdisplayOff();
void LCDcursorOn(bool blink);
void LCDclear();
void LCDWriteStringXY(int x,int y, const char *msg);
void LCDWriteIntXY(int x, int y, int var);
void LCDWriteFloatXY(int x, int y, float var);

//internal use only!!
void sendFullByte(uint rs, uint databits[]);
void sendRawDataOneCycle(uint raw_bits[]);
void uintInto8bits(uint raw_bits[], uint one_byte);
uint32_t pinValuesToMask(uint raw_bits[], int length);


#endif