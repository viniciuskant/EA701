#include "pico/stdlib.h"
#include "pico/time.h"
#include "stdio.h"
#include "hardware/pwm.h"

#define GPIO_14 14
#define GPIO_16 16

#define BOTAO_A 5
#define BOTAO_B 6
#define BOTAO_C 10

#define INCREMENTO_DUTY 0.1
#define MOD_DUTY 10


#define JOYSTICK_Y 26
#define JOYSTICK_X 27


void setup(){
    // gpio_init(GPIO_14); gpio_set_dir(GPIO_14, GPIO_OUT);
    gpio_init(GPIO_16); gpio_set_dir(GPIO_16, GPIO_IN);

    gpio_init(BOTAO_A); gpio_set_dir(BOTAO_A, GPIO_IN); gpio_pull_up(BOTAO_A);
    gpio_init(BOTAO_B); gpio_set_dir(BOTAO_B, GPIO_IN); gpio_pull_up(BOTAO_B);
    gpio_init(BOTAO_C); gpio_set_dir(BOTAO_C, GPIO_IN); gpio_pull_up(BOTAO_C);

}


void configure_pwm(uint slice_num, uint channel, uint16_t frequency, float duty_cycle) {
    uint32_t clock = 125000000; 
    uint32_t divider = clock / (frequency * 4096); 
    pwm_set_clkdiv(slice_num, divider);

    uint16_t wrap = clock / (divider * frequency) - 1; 
    pwm_set_wrap(slice_num, wrap);

    uint16_t level = (uint16_t)(wrap * duty_cycle); 
    pwm_set_chan_level(slice_num, channel, level);

    pwm_set_enabled(slice_num, true); 
}

int button_controller(uint pin, int *status, int *last_state) {
    int current_state = gpio_get(pin);
    if (current_state && !(*last_state)) {
        *status += 1;
        sleep_ms(50);
    }
    *last_state = current_state;
    return *status;
}

int main(){
    stdio_init_all();
    setup();

    uint slice_num = pwm_gpio_to_slice_num(GPIO_14);
    uint channel = pwm_gpio_to_channel(GPIO_14);
    gpio_set_function(GPIO_14, GPIO_FUNC_PWM);

    int status_botao[2] = {0, 0}, last_state[2] = {0, 0}, freq = 0;
    float duty = 0;
    
    while (true) {
        button_controller(BOTAO_A, &status_botao[0], &last_state[0]);
        button_controller(BOTAO_B, &status_botao[1], &last_state[1]);
        freq = status_botao[0] * 1000; 
        status_botao[1] = status_botao[1] % MOD_DUTY;
        duty = status_botao[1] * INCREMENTO_DUTY; 
        configure_pwm(slice_num, channel, freq, duty);
    }
}
