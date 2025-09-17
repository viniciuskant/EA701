import time, os, sys

def write_once(path, payload):
    t0 = time.ticks_us()
    with open(path, "wb") as f:
        f.write(payload)
        f.flush() 
    t1 = time.ticks_us()
    return time.ticks_diff(t1, t0)/1000

write_once("warmup.bin", b"\x00"*1024)
os.remove("warmup.bin")

time.sleep(5)
while True:
    for N in [32, 16, 8]:   
        data = bytes([i & 0xFF for i in range(N * 1024)])
        times = [write_once("data.bin", data) for _ in range(5)]
        print(f"Flash FS write ({N} kilobytes) ms: {times} média = {sum(times)/len(times)}")
        time.sleep(2)
