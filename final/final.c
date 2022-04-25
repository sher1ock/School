//Dan Jones, 4/25/22, added logic to be able to control the timer with buttons.

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/timer.h"
#include "hardware/gpio.h"
#include "hardware/adc.h"

/*
 * FourDigitLEDTest.c
 * Adapted from Arudio C code
 * Created: 4/20/2020 5:07:40 PM
 * Author : dframe (david.frame@uvu.edu)
 */ 

// Here are the hardware connections to the Pico:
#define LATCH 5     //74HC595  pin 12 RCLK
#define CLK 6       //74HC595  pin 11 SRCLK
#define DATA 7      //74HC595  pin 14 SER
#define THOUSANDS 0	//resistor->Four digit display 12 Low to enable
#define HUNDREDS 1	//resistor->Four digit display 9 Low to enable
#define TENS 2		//resistor->Four digit display 8 Low to enable
#define ONES 3		//resistor->Four digit display 6 Low to enable

#define ON_TIME 2
#define OFF_TIME 0
#define LOOPTIME 1

#define ELEMENTS(x) (sizeof(x)/sizeof((x)[0]))

#define PLAYPIN 28
#define RESETPIN 27

int play = 1; //set to 0 to have timer begin stopped.

int32_t count = 0;

bool repeating_timer_callback(struct repeating_timer *t) {
    if (play == true){
        if (count < 9999)
        {
            count++;
        }
        else
        {
            count = 0;
        }    
        return true;
    }
}

void pausetimer(uint gpio, uint32_t events){ //checks which gpio was triggered to know what to do.
    if (gpio == PLAYPIN){
        play = !play;

    }
    else{
        count = 0;
    }
}

void setup() {
    const uint outputs[] = {LATCH, CLK, DATA, THOUSANDS, HUNDREDS, TENS, ONES};
    const uint initval[] = {0,     0,   0,    1,         1,        1,    1 };
    stdio_init_all();
    for (int i = 0; i < ELEMENTS(outputs); i++){
        gpio_init(outputs[i]);
        gpio_set_dir(outputs[i],GPIO_OUT);
        gpio_put(outputs[i],initval[i]);
    }

    //pin setup, there's a better way of doing this but this is easier to follow.
    gpio_set_irq_enabled_with_callback(PLAYPIN, GPIO_IRQ_EDGE_FALL, true, &pausetimer);
    gpio_set_irq_enabled(RESETPIN, GPIO_IRQ_EDGE_FALL, true);
    gpio_init(PLAYPIN);
    gpio_init(RESETPIN);
    gpio_set_dir(PLAYPIN, false);
    gpio_set_dir(RESETPIN, false);
    gpio_pull_up(PLAYPIN);
    gpio_pull_up(RESETPIN);
}


void static inline shiftOut(char num)
{
	gpio_put(LATCH, false); //latch low
	for(char i = 0; i < 8;i++)
    {
		gpio_put(CLK, false);      //CLK low
		if ((num & 0x80) == 0x80)
        { // is the MSB a one?
			gpio_put(DATA, true);  // data pin HIGH
		}
		else
        {
			gpio_put(DATA, false); // data pin LOW
		}
        for (char j = 0; j < 13; j++)
        {
            asm("NOP;");    
        }
        //asm("NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP;"); //trying to meet 100ns setup time
		gpio_put(CLK,true);       //CLK pin high
        for (char k = 0; k < 7; k++)
        {
            asm("NOP;");
        }        
        //asm( "NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP;"); //trying to meet 100ns high time minimum for the clock
		num = num << 1;            // left shift to move the next bit to MSB
	}
	asm( "NOP; NOP; NOP;");
    gpio_put(LATCH, true); //high
	sleep_ms(1);
}

void static inline Display(unsigned int num)
{
    uint8_t digitEnable[] = {THOUSANDS,HUNDREDS,TENS,ONES};
    uint16_t digitSelect[]= {1000, 100, 10, 1};
    unsigned char table[]=
    {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,
        0x77,0x7c,0x39,0x5e,0x79,0x71,0x00};	
	int digit;
    char output;
	for(uint8_t i = 0; i < 4; i++)
    {		 
		digit = num / digitSelect[i];		//separate and select a digit
		num -= digit * digitSelect[i];
											
		gpio_put(digitEnable[i], false);	//enable digit
        if(i==1) //we are HUNDREDS
        {
            output = table[digit] | 0x80; //sets MSB of the output to display dp
        }
        else
        {
            output = table[digit];
        }
    	shiftOut(output); // display the digit
		sleep_ms(ON_TIME);
		gpio_put(digitEnable[i], 1);		//disable digit
	}
	sleep_ms(OFF_TIME);    
}


int main(void)
{
    // This sets a timer that updates count every 0.01 seconds - perfect 10ms from hardware
    setup();
    struct repeating_timer timer;
    add_repeating_timer_ms(10, repeating_timer_callback, NULL, &timer);
    while (1)
    {
        Display(count);
           
    }
}
