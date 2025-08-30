from machine import Pin
import time

led = Pin(13, Pin.OUT)
N = 5

while True:
    total_time = 0
    for i in range(N):
        start_time = time.ticks_us()
        
        led.value(1)
        time.sleep(0.5)
        led.value(0)
        time.sleep(0.5)
        
        end_time = time.ticks_us()
        total_time += time.ticks_diff(end_time, start_time)

    print("Tempo do loop:", total_time / N - 1000000, "us")
