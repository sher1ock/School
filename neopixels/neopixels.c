#include <stdio.h>
#include "pico/stdlib.h"
#include "../pico-examples/pio/ws2812/generated/ws2812.pio.h"
#include "hardware/adc.h"

// #include "hardware/clocks.h"

#define IS_RGBW false
#define NUM_PIXELS 12

int light = 1.2;
#define WS2812_PIN 4
#define repeat(x) for(int i = x; i--;)


static inline void put_pixel(uint32_t pixel_grb) {
    pio_sm_put_blocking(pio0, 0, pixel_grb << 8u);
}

static inline uint32_t urgb_u32(uint8_t r, uint8_t g, uint8_t b) {
    return
            ((uint32_t) (r) << 8) |
            ((uint32_t) (g) << 16) |
            (uint32_t) (b);
}



void bootanimation(){
    sleep_ms(500);
    int a=0;
    int b=0;
    int x=0;
    repeat(12){
        put_pixel(urgb_u32(0,10,0));

    }
    sleep_ms(100);
    while (a<13){
        repeat(x){
            put_pixel(urgb_u32(10,0,0));
            //x++;

        }
        a++;
        sleep_ms(100);
        x++;

    }
    x=0;
    while (b<13){
        repeat(x){
            put_pixel(urgb_u32(0,0,10));
            //x++;

        }
        b++;
        sleep_ms(100);
        x++;
    }
}


int main() {
    stdio_init_all();
    adc_init();
        // Make sure GPIO is high-impedance, no pullups etc
    adc_gpio_init(26);
    // Select ADC input 0 (GPIO26)
    adc_select_input(0);

    PIO pio = pio0;
    int sm = 0;
    uint offset = pio_add_program(pio, &ws2812_program);

    ws2812_program_init(pio, sm, offset, WS2812_PIN, 800000, IS_RGBW);
    //reset all pixels
    for (uint i=0; i<12; i++){
        put_pixel(0);
    }


    bootanimation();
    sleep_ms(1000);
    int x = 0;
    int z = 0;
    while (1){

        // put_pixel(urgb_u32(0,10,0));
        // sleep_ms(10);


        const float conversion_factor = 3.3f / (1 << 12);
        uint16_t result = adc_read();
        //printf("Raw value: 0x%03x, voltage: %f V\n", result, result * conversion_factor);
        z = (result*conversion_factor/3.3)*12;

        
        if (z>0){    
            while (x<z+1){
                x++;
                put_pixel(urgb_u32(0, 0, 5));

            }
        }
        
        while (z<12){
            z++;
            put_pixel(urgb_u32(20,0,0));
        }
        
           
        sleep_ms(50);
        x = 0;

    }
}