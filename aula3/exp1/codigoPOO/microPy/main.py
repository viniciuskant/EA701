from machine import Pin
import time

class LedRGB:
    def __init__(self, r=13, g=11, b=12):
        self.r = Pin(r, Pin.OUT)
        self.g = Pin(g, Pin.OUT)
        self.b = Pin(b, Pin.OUT)

    def set(self, r=0, g=0, b=0):
        self.r.value(r)
        self.g.value(g)
        self.b.value(b)

    def off(self):
        self.set(0,0,0)

led = LedRGB()

while True:
    time.sleep(1)
    start_time = time.ticks_us()  # Marca o tempo inicial
    led.set(1,0,0)  # vermelho
    led.off()
    end_time = time.ticks_us()  # Marca o tempo final
    time.sleep(1)

    elapsed_time = time.ticks_diff(end_time, start_time)  # Calcula a diferença
    print(f"Tempo desde ligar até desligar: {elapsed_time} us")