from machine import Pin
import utime

LED_R = 13
LED_G = 11
LED_B = 12

BUTTON_R = 5
BUTTON_G = 10
BUTTON_B = 6

class LedRGB:
    def __init__(self, r, g, b):
        self.r = Pin(r, Pin.OUT)
        self.g = Pin(g, Pin.OUT)
        self.b = Pin(b, Pin.OUT)

    def set(self, status):
        self.r.value(status[0])
        self.g.value(status[1])
        self.b.value(status[2])

def button_init(*pins):
    return [Pin(p, Pin.IN, Pin.PULL_UP) for p in pins]

def button_controller(pin, status, last_state):
    DEBOUNCE_MS = 50
    now = utime.ticks_ms()
    current_state = pin.value()
    if current_state == 1 and last_state[0] == 0: 
        if utime.ticks_diff(now, last_state[1]) >= DEBOUNCE_MS:
            status = 1 - status  
            last_state[1] = now  
    last_state[0] = current_state  
    return status

led = LedRGB(LED_R, LED_G, LED_B)
buttons = button_init(BUTTON_R, BUTTON_G, BUTTON_B)
status_led = [0, 1, 0]
last_state = [[1, 0], [1, 0], [1,0]]  

while True:
    for i in range(3):
        status_led[i] = button_controller(buttons[i], status_led[i], last_state[i])
    led.set(status_led)
