#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/clocks.h"
#include "hardware/gpio.h"
#include "hardware/adc.h"

const uint OUTPIN = 0;
const uint INPIN = 2;
const uint DELAYPIN = 26;

float DELAY = 3;

void gpio_callback(uint gpio, uint32_t events) {

    //gpio_put(OUTPIN, 0);
    busy_wait_us(DELAY);
    gpio_put(OUTPIN, 1);
    busy_wait_us(1000);
    gpio_put(OUTPIN, 0);
}

int main(){


    gpio_init(OUTPIN);
    gpio_init(INPIN);
    gpio_set_dir(INPIN, false);
    gpio_set_dir(OUTPIN, true);
    adc_init();
        // Make sure GPIO is high-impedance, no pullups etc
    adc_gpio_init(26);
    // Select ADC input 0 (GPIO26)
    adc_select_input(0);
    gpio_pull_down(OUTPIN);
    gpio_pull_up(INPIN);
    gpio_set_irq_enabled_with_callback(INPIN, GPIO_IRQ_EDGE_FALL, true, &gpio_callback);
    while (1){
        //busy_wait_ms(8);
        //gpio_put(OUTPIN, 0);
        // 12-bit conversion, assume max value == ADC_VREF == 3.3 V
        const float conversion_factor = 3.3f / (1 << 12);
        uint16_t result = adc_read();
        //printf("Raw value: 0x%03x, voltage: %f V\n", result, result * conversion_factor);
        DELAY = (result*conversion_factor/3.3)*8000;
        gpio_put(OUTPIN, 0);

    }
}

