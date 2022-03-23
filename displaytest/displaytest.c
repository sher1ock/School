#include <stdio.h>
#include <math.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "pico/binary_info.h"
#include "LCDops.h"
#include "generalOps.h"
#include "presetChars.h"
#include "presetMessages.h"




const uint DHT_PIN = 12;
const uint MAX_TIMINGS = 85;

typedef struct {
    float humidity;
    float temp_celsius;
} dht_reading;

void read_from_dht(dht_reading *result);


int LCDpins[14] = {0,1,2,3,4,5,6,7,15,16,17,16,2};

int main(){
    bi_decl(bi_program_description("This is a work-in-progress example of interfacing with LCD Displays using HD44780 chips on the Raspberry Pi Pico!"));

    uint8_t *buffer;
    gpio_init(DHT_PIN);
    stdio_init_all();

    //Initialize all needed pins as defined in LCDpins, set them as
    // outputs and then pull them low
    for(int gpio = 0; gpio < 11; gpio++){
        gpio_init(LCDpins[gpio]);
        gpio_set_dir(LCDpins[gpio], true);
        gpio_put(LCDpins[gpio], false);
    }

    //Initialize and clear the LCD, prepping it for characters / instructions
    LCDinit();

    while (1) {
        LCDclear();
        sleep_ms(8);
        LCDgoto("00");
        LCDsendRawInstruction(0,0,"00001100");
//        LCDwriteMessage("Hello World!");
        char buffer[33];
        dht_reading reading;
        read_from_dht(&reading);
        float fahrenheit = (reading.temp_celsius * 9 / 5) + 32;
        sprintf(buffer, "Humidity=%.1f%%, Temperature=%.1fC (%.1fF)\n",
               reading.humidity, reading.temp_celsius, fahrenheit);


        LCDwriteMessage(buffer);
        sleep_ms(5000);
        }
    
}



void read_from_dht(dht_reading *result) {
    uint8_t data[5] = {0, 0, 0, 0, 0};
    uint last = 1;
    uint j = 0;

    gpio_set_dir(DHT_PIN, GPIO_OUT);
    gpio_put(DHT_PIN, 0);
    sleep_ms(20);
    gpio_set_dir(DHT_PIN, GPIO_IN);


    //___            _____
    //    |___18ms___|
    //controller pulls low for 18-20us
    gpio_set_dir(DHT_PIN, GPIO_OUT);
    gpio_put(DHT_PIN, 0);
    sleep_ms(20); //18ms
    gpio_put(DHT_PIN, 1);

    //Now controller read the state of the pin
    gpio_set_dir(DHT_PIN, GPIO_IN);

    //___            __________
    //    |___18ms___| 20/40us |
    //wait for 0 (20-40us)
    int countWait = 0;
    for (uint i = 0; i < MAX_TIMINGS; i++) {
        if (gpio_get(DHT_PIN) == 0) {
            break;
        }
        countWait++;
        sleep_us(1);
    }

    //DHT answers by pulling low and high for 80us.
    //___            __________                        ____________________
    //   |___18ms___| 20/40us  |___DHT pulls low 80us__|   then high 80us   |

    //DHT pulls low 80us 
    int count0 = 0;
    for (uint i = 0; i < MAX_TIMINGS; i++) {
        if (gpio_get(DHT_PIN) != 0) {
            break;
        }
        count0++;
        sleep_us(1);
    }

    //DHT pulls high 80us 
    int count1 = 0;
    for (uint i = 0; i < MAX_TIMINGS; i++) {
        if (gpio_get(DHT_PIN) == 0) {
            break;
        }
        count1++;
        sleep_us(1);
    }
    
    
    char buffer[33];

    sprintf(buffer, "low=%d, high=%d", count0, count1);

    LCDwriteMessage(buffer);

    //DHT then send 40 bits of data
    //for each bit DHT pulls low for 50us 
    //then high for 26-28us for a zero
    //          for    70us for a one
    int nbBits = 0;
    int nbByte = 0;
    while (nbBits < 40) {
        //count 0 duration
        count0 = 0;
        for (uint i = 0; i < MAX_TIMINGS; i++) {
            if (gpio_get(DHT_PIN) != 0) {
                break;
            }
            count0++;
            sleep_us(1);
        }

        //count 1 duration
        count1 = 0;
        for (uint i = 0; i < MAX_TIMINGS; i++) {
            if (gpio_get(DHT_PIN) == 0) {
                break;
            }
            count1++;
            sleep_us(1);
        }   
        //if level up for more than 26-28us, it is a one
        if (count1 > 30)  {
           data[nbByte] = data[nbByte] | (1 << (7 - (nbBits%8))); 
        } 
        nbBits++;

        if ((nbBits%8) == 0)
            nbByte++;
    }


    if ((j >= 40) && (data[4] == ((data[0] + data[1] + data[2] + data[3]) & 0xFF))) {
        result->humidity = (float) ((data[0] << 8) + data[1]) / 10;
        if (result->humidity > 100) {
            result->humidity = data[0];
        }
        result->temp_celsius = (float) (((data[2] & 0x7F) << 8) + data[3]) / 10;
        if (result->temp_celsius > 125) {
            result->temp_celsius = data[2];
        }
        if (data[2] & 0x80) {
            result->temp_celsius = -result->temp_celsius;
        }
    } else {
        //LCDwriteMessage("Bad data");
    }
}