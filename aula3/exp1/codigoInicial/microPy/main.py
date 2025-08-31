from machine import Pin
import time

led = Pin(13, Pin.OUT)
N = 10
sleep = 0.5

while True:
    total_time = 0
    for i in range(N):
        time.sleep(sleep)
        start_time = time.ticks_us()
        led.value(1)
        led.value(0)
        end_time = time.ticks_us()
        time.sleep(sleep)
        total_time += time.ticks_diff(end_time, start_time)

    print("Tempo médio dos LEDs:", total_time / N, "us")
