#include "pico/stdlib.h"
#include "hardware/gpio.h"

#define LED 13
#define BTN 5

volatile bool led_state = false;
volatile absolute_time_t last_irq;

void btn_callback(uint gpio, uint32_t events) {
    if (gpio != BTN || !(events & GPIO_IRQ_EDGE_RISE)) return;
    int64_t us = absolute_time_diff_us(last_irq, get_absolute_time());
    if (us < 20000) return; // debounce ~20 ms
    last_irq = get_absolute_time();

    led_state = !led_state;
    gpio_put(LED, led_state);
}

int main() {
    stdio_init_all();
    gpio_init(LED); gpio_set_dir(LED, GPIO_OUT);
    gpio_init(BTN); gpio_set_dir(BTN, GPIO_IN); gpio_pull_up(BTN);

    // Registra o callback global
    gpio_set_irq_enabled_with_callback(BTN, GPIO_IRQ_EDGE_RISE, true, &btn_callback);

    while (true) {
        tight_loop_contents(); // espaço p/ outras tarefas ou RTOS
        for (volatile int i=0; i<128000; i++) {
            __asm volatile("nop"); // "no operation" - só gasta tempo
        }
    }
}