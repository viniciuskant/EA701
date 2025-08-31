from machine import Pin
import time

def setup_led(r_pin, g_pin, b_pin):
    return {
        "r": Pin(r_pin, Pin.OUT),
        "g": Pin(g_pin, Pin.OUT),
        "b": Pin(b_pin, Pin.OUT)
    }

def set_led(led, status):
    led["r"].value(status[0])
    led["g"].value(status[1])
    led["b"].value(status[2])

def setup_button(pin):
    return Pin(pin, Pin.IN, Pin.PULL_DOWN)

def is_button_pressed(button, last_state):
    current_state = button.value()
    if current_state and not last_state:
        last_state = 1
        while current_state:
            time.sleep(0.05)
            current_state = button.value()
        return True
    elif not current_state:
        last_state = 0
    return False

def main():
    LED_R, LED_G, LED_B = 13, 11, 12
    BUTTON_R, BUTTON_G, BUTTON_B = 5, 6, 22

    led = setup_led(LED_R, LED_G, LED_B)
    buttons = [setup_button(BUTTON_R), setup_button(BUTTON_G), setup_button(BUTTON_B)]
    button_states = [0, 0, 0] 
    status_led = [0, 0, 0]

    while True:
        for i, button in enumerate(buttons):
            if is_button_pressed(button, button_states):
                status_led[i] = 1 - status_led[i]
        set_led(led, status_led)
        time.sleep(0.05)

if __name__ == "__main__":
    main()