#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "stdio.h"
#include "stdlib.h"

#define LED_R 13
#define LED_G 11
#define LED_B 12

#define BUTTON_R 5
#define BUTTON_G 6
#define BUTTON_B 10

#define BUFFER_SIZE 20

typedef struct {
    uint r, g, b;
} LedRGB;

LedRGB led;
int status_led[3] = {0, 0, 0};

int sensor_buffers[3][BUFFER_SIZE] = {0};
int index[3] = {0,0,0};

void button_init(uint rp, uint gp, uint bp){
    gpio_init(rp); gpio_set_dir(rp, GPIO_IN); gpio_pull_up(rp);
    gpio_init(gp); gpio_set_dir(gp, GPIO_IN); gpio_pull_up(gp);
    gpio_init(bp); gpio_set_dir(bp, GPIO_IN); gpio_pull_up(bp);
}


void led_init(LedRGB* L, uint rp, uint gp, uint bp){
    L->r = rp; L->g = gp; L->b = bp;
    gpio_init(rp); gpio_set_dir(rp, GPIO_OUT);
    gpio_init(gp); gpio_set_dir(gp, GPIO_OUT);
    gpio_init(bp); gpio_set_dir(bp, GPIO_OUT);
}

void led_set(LedRGB* L, int status[]){
    gpio_put(L->r, status[0]);
    gpio_put(L->g, status[1]);
    gpio_put(L->b, status[2]);
}

int compare_int(const void* a, const void* b) {
    return (*(int*)a - *(int*)b);
}

void iqr_bounds(int* buffer, int n, int* low, int* high) {
    int sorted[BUFFER_SIZE];
    for (int i = 0; i < n; i++) sorted[i] = buffer[i];
    qsort(sorted, n, sizeof(int), compare_int);

    int q1 = sorted[n / 4];
    int q3 = sorted[(3 * n) / 4];
    int iqr = q3 - q1;

    *low = q1 - (int)(1.5 * iqr);
    *high = q3 + (int)(1.5 * iqr);
}

int led_status_from_iqr(int val, int* buffer, int n){
    int low, high;
    iqr_bounds(buffer, n, &low, &high);
    return (val > high) ? 1 : 0;
}

void button_callback(uint gpio, uint32_t events) {
    if (gpio == BUTTON_R) status_led[0] ^= 1;
    else if (gpio == BUTTON_G) status_led[1] ^= 1;
    else if (gpio == BUTTON_B) status_led[2] ^= 1;

    led_set(&led, status_led);
}

void setup_gpio_interrupts() {
    gpio_set_irq_enabled_with_callback(BUTTON_R, GPIO_IRQ_EDGE_FALL, true, &button_callback);
    gpio_set_irq_enabled_with_callback(BUTTON_G, GPIO_IRQ_EDGE_FALL, true, &button_callback);
    gpio_set_irq_enabled_with_callback(BUTTON_B, GPIO_IRQ_EDGE_FALL, true, &button_callback);
}

int main() {
    stdio_init_all();
    led_init(&led, LED_R, LED_G, LED_B);
    button_init(BUTTON_R, BUTTON_G, BUTTON_B);
    int pin_BUTTON[3] = {BUTTON_R, BUTTON_G, BUTTON_B};


    setup_gpio_interrupts();

    while(1) {
        for (int i = 0; i < 3; i++) {
            int val = gpio_get(pin_BUTTON[i]);
            sensor_buffers[i][index[i]] = val;
            index[i] = (index[i] + 1) % BUFFER_SIZE;

            status_led[i] = led_status_from_iqr(val, sensor_buffers[i], BUFFER_SIZE);
        }

        led_set(&led, status_led);
        sleep_ms(200);
    }
}
