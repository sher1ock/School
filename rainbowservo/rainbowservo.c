#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "ws2812.pio.h"
#include "hardware/adc.h"


#define RED 0, 0xff, 0
#define GREEN 0xff, 0, 0
#define BLUE 0, 0, 0xff
#define Yellow 0xff, 0xff, 0
#define WHITE 0xff, 0xff, 0xff

#define servopin 20
#define inputpin 27

#define IS_RGBW false
#define NUM_PIXELS 12
#define brightness 50

uint16_t adcresult = 1;

int light = 1.2;
#define WS2812_PIN 2
#define repeat(x) for(int i = x; i--;)

uint32_t pwm_set_freq_duty(uint slice_num, uint chan, uint32_t f, int d)
{
    uint32_t clock = 125000000;
    uint32_t divider16 = clock / f / 4096 + (clock % (f * 4096) != 0);
    if (divider16 / 16 == 0)
        divider16 = 16;
    uint32_t wrap = clock * 16 / divider16 / f - 1;
    pwm_set_clkdiv_int_frac(slice_num, divider16 / 16, divider16 & 0xF);
    pwm_set_wrap(slice_num, wrap);
    pwm_set_chan_level(slice_num, chan, wrap * d / 100);
    return wrap;
}
uint32_t pwm_get_wrap(uint slice_num)
{
    valid_params_if(PWM, slice_num >= 0 && slice_num < NUM_PWM_SLICES);
    return pwm_hw->slice[slice_num].top;
}

void pwm_set_duty(uint slice_num, uint chan, int d)
{
    pwm_set_chan_level(slice_num, chan, pwm_get_wrap(slice_num) * d / 100);
}
void pwm_set_dutyH(uint slice_num, uint chan, int d)
{
    pwm_set_chan_level(slice_num, chan, pwm_get_wrap(slice_num) * d / 10000);
}

typedef struct
{
    uint gpio;
    uint slice;
    uint chan;
    uint speed;
    uint resolution;
    bool on;
    bool invert;
} Servo;

void ServoInit(Servo *s, uint gpio, bool invert)
{
    gpio_set_function(servopin, GPIO_FUNC_PWM);
    s->gpio = gpio;
    s->slice = pwm_gpio_to_slice_num(gpio);
    s->chan = pwm_gpio_to_channel(gpio);

    pwm_set_enabled(s->slice, false);
    s->on = false;
    s->speed = 0;
    s->resolution = pwm_set_freq_duty(s->slice, s->chan, 50, 0);
    pwm_set_dutyH(s->slice, s->chan, 250);
    if (s->chan)
    {
        pwm_set_output_polarity(s->slice, false, invert);
    }
    else
    {
        pwm_set_output_polarity(s->slice, invert, false);
    }
    s->invert = invert;
}

void ServoOn(Servo *s)
{
    pwm_set_enabled(s->slice, true);
    s->on = true;
}

void ServoOff(Servo *s)
{
    pwm_set_enabled(s->slice, false);
    s->on = false;
}
void ServoPosition(Servo *s, uint p)
{
    pwm_set_dutyH(s->slice, s->chan, p * 10 + 250);
}

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

void rainbow(volatile color){
    int r,g,b;
    // 255, 510, 765, 1020

    if (color <= 255){
        r = 255;
        g = color;
        b = 0;
    }
    if(color > 255){
        r = 510 - color;
        g = 255;
        b = 0;
        
    }
    if(color > 510){
        r = 0;
        g = 255;
        b = color - 510;
    }

    if(color > 765){
        r = 0;
        g = 1020 - color;
        b = 255;

    }

    r = r * 1/brightness;
    g = g * 1/brightness;
    b = b* 1/brightness;

    
    repeat(12){
        put_pixel(urgb_u32(r,g,b));
    }




}

int main()
{   
    stdio_init_all();
    adc_init();
    adc_gpio_init(27);
    adc_select_input(1);

    Servo s1;
    ServoInit(&s1, 20, false);

    ServoOn(&s1);




    PIO pio = pio0;
    int sm = 0;
    uint offset = pio_add_program(pio, &ws2812_program);

    ws2812_program_init(pio, sm, offset, WS2812_PIN, 800000, IS_RGBW);
    //reset all pixels
    for (uint i=0; i<12; i++){
        put_pixel(0);
    }

    
    bootanimation();

    ServoPosition(&s1, 50);
    sleep_ms(500);


    while (true)
        {
            const float conversion_factor = 1.01f / (1 << 12);
            uint16_t result = adc_read();
            int servodes = result * conversion_factor * 100;
            int color = result * conversion_factor * 1020;



            // repeat(12){
            //     put_pixel(urgb_u32(leddes,0,0));
            // }
            rainbow(color);
            ServoPosition(&s1, servodes);
            sleep_ms(50);
            // ServoPosition(&s1, 100);
            // sleep_ms(500);
        }    
        

    return 0;
}
