#include "pico/stdlib.h"
#include "pico/time.h"
#include "stdio.h"

#define LED_PIN 13
#define N 1
#define SLEEP 500

int main() {

    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    stdio_init_all(); // por algum motivo se não tinha isso a serial não funcionava
    
    while(1){
        uint32_t total_time = 0;
        for (int i = 0; i < N; i++) {
            absolute_time_t start_time = get_absolute_time();

            gpio_put(LED_PIN, 1);
            sleep_ms(SLEEP);
            gpio_put(LED_PIN, 0);
            sleep_ms(SLEEP);

            absolute_time_t end_time = get_absolute_time();
            total_time += absolute_time_diff_us(start_time, end_time);
        }

        uint32_t average_time = total_time / N;
        printf("Tempo médio para ligar e desligar o LED: %u us\n", average_time - 2000 * SLEEP);
    }
    return 0;
}

