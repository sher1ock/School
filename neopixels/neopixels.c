#include <stdio.h>
#include "pico/stdlib.h"
#include "../pico-examples/pio/ws2812/generated/ws2812.pio.h"
// #include "hardware/clocks.h"

#define IS_RGBW false
#define NUM_PIXELS 12

#ifdef PICO_DEFAULT_WS2812_PIN
#define WS2812_PIN PICO_DEFAULT_WS2812_PIN
#else
// default to pin 2 if the board doesn't have a default WS2812 pin defined
#define WS2812_PIN 4
#endif

static inline void put_pixel(uint32_t pixel_grb) {
    pio_sm_put_blocking(pio0, 0, pixel_grb << 8u);
}

static inline uint32_t urgb_u32(uint8_t r, uint8_t g, uint8_t b) {
    return
            ((uint32_t) (r) << 8) |
            ((uint32_t) (g) << 16) |
            (uint32_t) (b);
}

// void pattern_snakes(uint len, uint t) {
//     for (uint i = 0; i < len; ++i) {
//         uint x = (i + (t >> 1)) % 64;
//         if (x < 10)
//             put_pixel(urgb_u32(0xff, 0, 0));
//         else if (x >= 15 && x < 25)
//             put_pixel(urgb_u32(0, 0xff, 0));
//         else if (x >= 30 && x < 40)
//             put_pixel(urgb_u32(0, 0, 0xff));
//         else
//             put_pixel(0);
//     }
// }

// void pattern_random(uint len, uint t) {
//     if (t % 8)
//         return;
//     for (int i = 0; i < len; ++i)
//         put_pixel(rand());
// }

// void pattern_sparkle(uint len, uint t) {
//     if (t % 8)
//         return;
//     for (int i = 0; i < len; ++i)
//         put_pixel(rand() % 16 ? 0 : 0xffffffff);
// }

// void pattern_greys(uint len, uint t) {
//     int max = 100; // let's not draw too much current!
//     t %= max;
//     for (int i = 0; i < len; ++i) {
//         put_pixel(t * 0x10101);
//         if (++t >= max) t = 0;
//     }
// }
void pattern_test(uint len, uint t) {
    for (uint i = 0; i < len; ++i) {
        uint x = (i + (t >> 1)) % 64;
        if (x < 12)
            put_pixel(urgb_u32(5, 0, 0));
        // else if (x >= 12 && x < 24)
        //     put_pixel(urgb_u32(0, 5, 0));
        else if (x >= 24 && x < 36)
            put_pixel(urgb_u32(0, 0, 5));
        //else
        //    put_pixel(0);
    }
}

// typedef void (*pattern)(uint len, uint t);
// const struct {
//     pattern pat;
//     const char *name;
// } pattern_table[] = {
//         //{pattern_snakes,  "Snakes!"},
//         //{pattern_random,  "Random data"},
//         //{pattern_sparkle, "Sparkles"},
//         //{pattern_greys,   "Greys"},
//         {pattern_test,    "test"},
// };
int main() {
    stdio_init_all();
    printf("WS2812 Smoke Test, using pin %d", WS2812_PIN);

    PIO pio = pio0;
    int sm = 0;
    uint offset = pio_add_program(pio, &ws2812_program);

    ws2812_program_init(pio, sm, offset, WS2812_PIN, 800000, IS_RGBW);

    int t = 0;
    int dir = 1;
    int r = 0;
    int g = 0;
    int b = 0;
    while (1){
        for(int i=0;i<NUM_PIXELS;i++){
   
            while (r < 30) {
                r++;
                if (b>0) {
                    b--;
                }
            put_pixel(urgb_u32(r, g, b));
            sleep_ms(10);
            }

            while (g < 30) {
                g++;
                if (r>0) {
                    r--;
                }
                put_pixel(urgb_u32(r, g, b));
                sleep_ms(10);
            }

            while (b < 30){
                b++;
                if (g>0) {
                    g--;
                }
                put_pixel(urgb_u32(r, g, b));
                sleep_ms(10);
            }

        }


    }
    
}
