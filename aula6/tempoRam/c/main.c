#include "pico/stdlib.h"
#include "stdio.h"
#include <pico/time.h>

#define N  10000

int main() {
    stdio_init_all();
    static uint8_t buf[10000];

    while (1) {
        absolute_time_t t0 = get_absolute_time();
        for (int i = 0; i < N; i++) 
        buf[i] = i & 0xFF;
        absolute_time_t t1 = get_absolute_time();
        
        int64_t us = absolute_time_diff_us(t0, t1);
        printf("RAM write (%d bytes): %.3f ms\n", N, us/1000.0);
        
        sleep_ms(2000);
    }

}