#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/pwm.h"
#include "pwmPID.h"

uint slice_num, channel;

void pwmPID_init(uint pin){
    gpio_set_function(pin, GPIO_FUNC_PWM);
    slice_num = pwm_gpio_to_slice_num(pin);
    pwm_set_wrap(slice_num, PWM_SLICE); //16 bits
    channel = pwm_gpio_to_channel(pin);
    pwm_set_chan_level(slice_num, channel, 0); //start at zero
    pwm_set_enabled(slice_num, true);
}

void pwmPID_output(double value){
    pwm_set_chan_level(slice_num, channel, (uint16_t)value);
}