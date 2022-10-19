#include "MAX6675.h"

max6675_t Max;

void MAX6675Init(int SCLK, int MISO, int CS)
{
    gpio_init(MISO);
    gpio_set_dir(MISO,GPIO_IN);
	gpio_disable_pulls(MISO);
	gpio_set_input_hysteresis_enabled(MISO, true);

    gpio_init(SCLK);
    gpio_set_dir(SCLK,GPIO_OUT);

    gpio_init(CS);
    gpio_set_dir(CS,GPIO_OUT);

    //initial values for outputs
    gpio_put(CS, 1);
    gpio_put(SCLK, 1);
    
    // set the global values
    Max.CS = CS;
    Max.CLK = SCLK;
    Max.MISO = MISO;

    busy_wait_ms(500);

}

uint SPIRead16(void)
{
    int i;
    uint d = 0;

    for(i = 15; i >= 0; i--)
    {
        gpio_put(Max.CLK, 0);
        busy_wait_us(100);
        if (gpio_get(Max.MISO))
        {
            d |= (1 << i);
        }
        gpio_put(Max.CLK, 1);
        busy_wait_us(100);
    }
    return d;
}

float readCelsius(void)
{
    uint v;
    gpio_put(Max.CS, 0);
    busy_wait_us(100);

    v = SPIRead16();
    gpio_put(Max.CS, 1);

    if(v & 0x4)
    {
        // no thermocouple attached
        return -1;
    }
    
    v >>= 3;
    return (float)v * 0.25;
}

float readFahrenheit(void) { return readCelsius() * 9.0 / 5.0 + 32; }