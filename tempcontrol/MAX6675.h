//for the MAX6675.c file
#ifndef MAX6675HEADER
    #define MAX6675HEADER
    #include "stdlib.h"
    #include <stdio.h>
    #include "pico/stdlib.h"

    #define MAX6675_SCLK 8
    #define MAX6675_MISO 7
    #define MAX6675_CS 6

    typedef struct max6675Interface
    {
        int CLK;
        int MISO;
        int CS;
    }max6675_t;

    void MAX6675Init(int SCLK, int MISO, int CS);
    uint SPIRead16(void);
    float readCelsius(void);
    float readFahrenheit(void);
#endif    
