from machine import Pin
import utime
import random

class LedRGB:
    def __init__(self, r, g, b):
        self.r = Pin(r, Pin.OUT)
        self.g = Pin(g, Pin.OUT)
        self.b = Pin(b, Pin.OUT)

    def set(self, status):
        self.r.value(status[0])
        self.g.value(status[1])
        self.b.value(status[2])

class Sensor:
    def __init__(self, pin, buffer_size=20):
        self.pin = pin
        self.buffer = [0] * buffer_size
        self.buffer_size = buffer_size

    def update_buffer(self, value):
        self.buffer.append(value)
        if len(self.buffer) > self.buffer_size:
            self.buffer.pop(0)

    def iqr_bounds(self):
        sorted_vals = sorted(self.buffer)
        n = len(sorted_vals)
        q1 = sorted_vals[n // 4]
        q3 = sorted_vals[(3 * n) // 4]
        iqr = q3 - q1
        low = q1 - 1.5 * iqr
        high = q3 + 1.5 * iqr
        return low, high

    def led_status_from_iqr(self, value):
        low, high = self.iqr_bounds()
        return 1 if value > high else 0

class Button:
    def __init__(self, pin):
        self.pin = Pin(pin, Pin.IN, Pin.PULL_UP)

    def is_pressed(self):
        return self.pin.value() == 0

class System:
    def __init__(self, led_pins, button_pins, buffer_size=20):
        self.led = LedRGB(*led_pins)
        self.sensors = [Sensor(pin, buffer_size) for pin in range(3)]
        self.buttons = [Button(pin) for pin in button_pins]
        self.status_leds = [0, 0, 0]

    def update(self):
        for i, sensor in enumerate(self.sensors):
            val = random.randint(0, 100)
            sensor.update_buffer(val)
            self.status_leds[i] = sensor.led_status_from_iqr(val)

            if self.buttons[i].is_pressed():
                self.status_leds[i] ^= 1

        self.led.set(self.status_leds)

def main():
    LED_PINS = (13, 11, 12)
    BUTTON_PINS = (5, 10, 6)
    system = System(LED_PINS, BUTTON_PINS)

    while True:
        system.update()
        utime.sleep_ms(200)

if __name__ == "__main__":
    main()
