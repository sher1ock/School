#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/clocks.h"

const uint OUTPIN = 0;
const uint INPIN = 2;

int DELAY = 5;

static void gpio_pull_up(uint gpio);


void gpio_callback(uint gpio, uint32_t events) {

    //gpio_put(OUTPIN, 0);
    sleep_ms(DELAY);
    gpio_put(OUTPIN, 1);
    sleep_ms(2);
    gpio_put(OUTPIN, 0);

}

int main(){


    clocks_init();
    stdio_init_all();
    gpio_pull_down(OUTPIN);
    //gpio_put(OUTPIN, 0);
    gpio_set_irq_enabled_with_callback(INPIN, GPIO_IRQ_EDGE_RISE, true, &gpio_callback);
    // while (1){
    //     sleep_ms(10);
    //     //gpio_put(OUTPIN, 0);
    // }
}

