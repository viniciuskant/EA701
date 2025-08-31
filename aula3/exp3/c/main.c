#include "pico/stdlib.h"
#include "pico/time.h"
#include "stdio.h"

#define LED_R 13
#define LED_G 11
#define LED_B 12

#define BOTTON_R 5
#define BOTTON_G 6
#define BOTTON_B 22

typedef struct {
    uint r, g, b;
} LedRGB;

void led_init(LedRGB* L, uint rp, uint gp, uint bp){
    L->r = rp; L->g = gp; L->b = bp;
    gpio_init(rp); gpio_set_dir(rp, GPIO_OUT);
    gpio_init(gp); gpio_set_dir(gp, GPIO_OUT);
    gpio_init(bp); gpio_set_dir(bp, GPIO_OUT);
}

void botton_init(uint rp, uint gp, uint bp){
    gpio_init(rp); gpio_set_dir(rp, GPIO_IN);
    gpio_init(gp); gpio_set_dir(gp, GPIO_IN);
    gpio_init(bp); gpio_set_dir(bp, GPIO_IN);
}


void led_set(LedRGB* L, int status[]){
    gpio_put(L->r, status[0]); 
    gpio_put(L->g, status[1]); 
    gpio_put(L->b, status[2]);
}

int botton_controller (uint pin, int status){
    int status_botton = 0;
    while (gpio_get(pin)){
        if (!status_botton){
            status_botton = 1;
            status = 1 - status;
        }
        sleep_ms(50);
    }
    return status;
}

int main(){
    stdio_init_all();
    int status_led[3] = {0, 0, 0};
    int pin_botton[3] = {BOTTON_R, BOTTON_G, BOTTON_B};
    
    LedRGB led;
    led_init(&led, LED_R, LED_G, LED_B);
    botton_init(BOTTON_R, BOTTON_G, BOTTON_B);
    

    while(1){
        for (int i = 0; i < 3; i++)
            botton_controller(pin_botton[i], status_led[i]);
        led_set(&led,status_led); 
    }
}
