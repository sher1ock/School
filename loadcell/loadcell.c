
#include <stdlib.h>
#include <stdio.h>
#include "pico/stdio.h"
#include "hx711.h"
#include "hx711_noblock.pio.h"

int main(void) {

    stdio_init_all();

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