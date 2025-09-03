#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "stdio.h"

#define LED_R 13
#define LED_G 11
#define LED_B 12

#define BUTTON_R 5
#define BUTTON_G 6
#define BUTTON_B 10

typedef struct {
    uint r, g, b;
} LedRGB;

void led_init(LedRGB* L, uint rp, uint gp, uint bp){
    L->r = rp; L->g = gp; L->b = bp;
    gpio_init(rp); gpio_set_dir(rp, GPIO_OUT);
    gpio_init(gp); gpio_set_dir(gp, GPIO_OUT);
    gpio_init(bp); gpio_set_dir(bp, GPIO_OUT);
}

void button_init(uint rp, uint gp, uint bp){
    gpio_init(rp); gpio_set_dir(rp, GPIO_IN); gpio_pull_up(rp);
    gpio_init(gp); gpio_set_dir(gp, GPIO_IN); gpio_pull_up(gp);
    gpio_init(bp); gpio_set_dir(bp, GPIO_IN); gpio_pull_up(bp);
}

void led_set(LedRGB* L, int status[]){
    gpio_put(L->r, status[0]);
    gpio_put(L->g, status[1]);
    gpio_put(L->b, status[2]);
}

int button_controller(uint pin, int status, int *last_state) {
    int current_state = gpio_get(pin);
    if (current_state && !(*last_state)) {
        status = 1 - status;
        sleep_ms(50);
    }
    *last_state = current_state;
    return status;
}

int main() {
    stdio_init_all();
    LedRGB led;
    led_init(&led, LED_R, LED_G, LED_B);
    button_init(BUTTON_R, BUTTON_G, BUTTON_B);
    int status_led[3] = {0, 1, 0};
    int last_state[3] = {0, 0, 0};
    int pin_BUTTON[3] = {BUTTON_R, BUTTON_G, BUTTON_B};
    
    while (1) {
        for (int i = 0; i < 3; i++)
            status_led[i] = button_controller(pin_BUTTON[i], status_led[i], &last_state[i]);
        led_set(&led, status_led);
    }
}
