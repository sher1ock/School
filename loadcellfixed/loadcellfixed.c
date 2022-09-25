
#include <stdlib.h>
#include <stdio.h>
#include "pico/stdio.h"
#include "hx711-pico-c/include/hx711.h"
#include "hx711-pico-c/include/hx711_noblock.pio.h"
#include "ws2812.pio.h"


#define IS_RGBW false
#define NUM_PIXELS 12
#define brightness 50

uint16_t adcresult = 1;

int light = 1.2;
#define WS2812_PIN 0
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
    sleep_ms(250);
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
        sleep_ms(50);
        x++;
    }
}


int main(void) {

    stdio_init_all();



    PIO pio = pio0;
    int sm = 0;
    uint offset = pio_add_program(pio, &ws2812_program);

    ws2812_program_init(pio, sm, offset, WS2812_PIN, 800000, IS_RGBW);
    //reset all pixels
    for (uint i=0; i<12; i++){
        put_pixel(0);
    }

    
    bootanimation();


    // SET THESE TO THE GPIO PINS CONNECTED TO THE
    // HX711's CLOCK AND DATA PINS
    // PINOUT REFERENCE: https://learn.adafruit.com/assets/99339
    const uint clkPin = 14; // GP14, PAD19
    const uint datPin = 15; // GP15, PAD20

    hx711_t hx;

    //1. init the hx711 struct
    hx711_init(
        &hx,
        clkPin,
        datPin,
        pio0,
        &hx711_noblock_program,
        &hx711_noblock_program_init);

    //2. power up the hx711
    hx711_set_power(&hx, hx711_pwr_up);

    //3. [OPTIONAL] set gain and save it to the HX711
    //chip by powering down then back up
    hx711_set_gain(&hx, hx711_gain_128);
    hx711_set_power(&hx, hx711_pwr_down);
    hx711_wait_power_down();
    hx711_set_power(&hx, hx711_pwr_up);

    //4. pause to allow the readings to settle based on the
    //sample rate of the chip
    hx711_wait_settle(hx711_rate_80);

    //at this point, the hx711 can reliably produce values
    //with hx711_get_value or hx711_get_value_timeout

    for(;;) {
        printf("%li\n", hx711_get_value(&hx));
    }

    return EXIT_SUCCESS;

}
