#include "pico/stdlib.h"
#include "pico/time.h"
#include "stdio.h"


typedef struct {
    uint r, g, b;
} LedRGB;

void led_init(LedRGB* L, uint rp, uint gp, uint bp){
    L->r = rp; L->g = gp; L->b = bp;
    gpio_init(rp); gpio_set_dir(rp, GPIO_OUT);
    gpio_init(gp); gpio_set_dir(gp, GPIO_OUT);
    gpio_init(bp); gpio_set_dir(bp, GPIO_OUT);
}

void led_set(LedRGB* L, bool r, bool g, bool b){
    gpio_put(L->r, r); gpio_put(L->g, g); gpio_put(L->b, b);
}

int main(){
    stdio_init_all();
    LedRGB led;
    led_init(&led, 13, 13, 13);
    while (true)
    {
        /* code */
        
        absolute_time_t start_time = get_absolute_time();
        
        led_set(&led,1,0,0); sleep_ms(500);
        led_set(&led,0,1,0); sleep_ms(500);
        
        absolute_time_t end_time = get_absolute_time();

        printf("Tempo médio para ligar e desligar o LED: %lld us\n", absolute_time_diff_us(start_time,end_time) - 1000000);

        led_set(&led,0,0,1); sleep_ms(500);
        led_set(&led,0,0,0);
    }
}