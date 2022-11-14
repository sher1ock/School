/*Encoder GPIO*/
// GPIO 21 is Encoder phase A,  
// GPIO 20 is Encoder phase B,
// GPIO 19 is the encoder push botton switch.
// change these as needed
#include "stdlib.h"
#include <stdio.h>
#include "pico/stdlib.h"

#ifndef ENCODERHEADER
    #define ENCODERHEADER
    #define EN_SPEED 6
    #define EN_CLK	21
    #define EN_DT	20
    #define EN_SW	19
    #define MINIMUM 0
    #define MAXIMUM 9999
    #define ELEMENTS(x) (sizeof(x)/sizeof((x)[0]))
    void Encoder_init(void);
    void encoder_callback(uint gpio, uint32_t events); 
#endif    