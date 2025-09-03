#include "pico/stdlib.h"
#include "pico/time.h"
#include <stdio.h>
#include "hardware/pwm.h"
#include "hardware/adc.h"


#define GPIO_14 14
#define GPIO_16 16

#define BOTAO_A 5
#define BOTAO_B 6
#define BOTAO_C 10

#define INCREMENTO_DUTY 0.025
#define MOD_DUTY 40


#define JOYSTICK_Y 26
#define JOYSTICK_X 27
#define JOYSTICK_B 22


void setup_pin(){
    gpio_init(GPIO_16); gpio_set_dir(GPIO_16, GPIO_IN);

    gpio_init(BOTAO_A); gpio_set_dir(BOTAO_A, GPIO_IN); gpio_pull_up(BOTAO_A);
    gpio_init(BOTAO_B); gpio_set_dir(BOTAO_B, GPIO_IN); gpio_pull_up(BOTAO_B);
    gpio_init(BOTAO_C); gpio_set_dir(BOTAO_C, GPIO_IN); gpio_pull_up(BOTAO_C);
    gpio_init(JOYSTICK_B); gpio_set_dir(JOYSTICK_B, GPIO_IN); gpio_pull_up(JOYSTICK_B);

}

uint16_t read_analog(uint pin) {
    adc_gpio_init(pin); 
    adc_select_input(pin - 26); 
    return adc_read(); 
}

void configure_pwm(uint slice_num, uint channel, uint32_t frequency, float duty_cycle) {
    uint32_t clock = 125000000; 
    uint32_t divider = clock / (frequency * 4096); 
    pwm_set_clkdiv(slice_num, divider);

    uint16_t wrap = clock / (divider * frequency) - 1; 
    pwm_set_wrap(slice_num, wrap);

    uint16_t level = (uint16_t)(wrap * duty_cycle); 
    pwm_set_chan_level(slice_num, channel, level);

    pwm_set_enabled(slice_num, true); 
}

int conter_edge(uint pin, int *status, int *last_state, int incremento) {
    int current_state = gpio_get(pin);
    if (current_state && !(*last_state)) {
        if (incremento) 
            *status += 1;
        else
            *status -= 1;
        sleep_ms(50);
    }
    *last_state = current_state;
    return *status;
}

int button_controller(uint pin, int *status, int *last_state, int incremento, int time) {
    int current_state = gpio_get(pin);
    if (current_state && !(*last_state)) {
        if (incremento) 
            *status += 1;
        else
            *status -= 1;
        sleep_ms(time);
    }
    *last_state = current_state;
    return *status;
}



int main(){
    stdio_init_all();
    adc_init();
    setup_pin();

    uint slice_num = pwm_gpio_to_slice_num(GPIO_14);
    uint channel = pwm_gpio_to_channel(GPIO_14);
    gpio_set_function(GPIO_14, GPIO_FUNC_PWM);

    int status_button[5], last_state[5], freq = 0;
    for (int i = 0; i < 5; i++){
        status_button[i] = 0;
        last_state[i] = 0;
    }

    float duty = 0;
    int counter = 0;

    while (true) {
        button_controller(BOTAO_A, &status_button[0], &last_state[0], status_button[3] % 2, 50);
        button_controller(BOTAO_B, &status_button[1], &last_state[1], status_button[3] % 2, 50);
        button_controller(BOTAO_C, &status_button[2], &last_state[2], 1, 50);
        button_controller(JOYSTICK_B, &status_button[3], &last_state[3], 1, 50);
        
        button_controller(GPIO_16, &status_button[4], &last_state[4], 1, 0);
        
        
        if (status_button[2] % 2) {
            freq = read_analog(JOYSTICK_X);
            duty = (float)read_analog(JOYSTICK_Y) / 4096.0;
        }
        if (duty + status_button[1] * INCREMENTO_DUTY >= 1)
            status_button[1] = 0;

        configure_pwm(slice_num, channel, freq + status_button[0] * 100, duty + status_button[1] * INCREMENTO_DUTY);


        static absolute_time_t last_print_time = {0};
        if (absolute_time_diff_us(last_print_time, get_absolute_time()) >= 1000000) {
            printf("Duty Cycle: %.2f, Frequency: %d, GPIO Frequency: %d\n", duty + status_button[1] * INCREMENTO_DUTY, freq, status_button[4] - counter);
            last_print_time = get_absolute_time();
            counter = status_button[4];
        }
    }
}
