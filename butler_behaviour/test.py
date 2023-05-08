#!/usr/bin/python3

import serial
import time
from time import sleep, monotonic
import signal

class Butler:
    def __init__(self):
        self.ser = serial.Serial()
        self.ser.baudrate = 115200
        self.ser.port = "/dev/ttyACM0"
        self.ser.timeout = 0.020
        self.ser.open()
        time.sleep(2) # Arduino will reset when the serial port is opened.
        self.set_leds(self.CYAN)

    def stop(self):
        self.ser.write("m 0 0\r".encode())
        self.ser.flush()
        print(self.ser.readline())

    def forward(self, speed = 520):
        self.ser.write("m {0} {0}\r".format(speed).encode())
        self.ser.flush()
        self.ser.readline()

    def turn_left(self):
        self.ser.write(b"m -500 500\r")
        self.ser.flush()
        self.ser.readline()

    def get_ultrasonics(self):
        self.ser.write("u\r".encode())
        self.ser.read_until("{".encode())
        data = self.ser.read_until("}".encode())
        return [int(i) for i in data.decode().strip("{}").split(" ")]

    # Colors
    GREEN = (0,2048,0)
    YELLOW = (2048,1500,0)
    RED = (2048,0,0)
    CYAN = (0,1024,1024)
    OFF = (0,0,0) 
    def set_leds(self, color):
        (red, green, blue) = color
        self.ser.write("o 4 {}\r".format(green).encode())
        self.ser.write("o 5 {}\r".format(red).encode())
        self.ser.write("o 6 {}\r".format(blue).encode())
        self.ser.flush()



if __name__ == "__main__":
    # Connect to hardware interface
    try:
        butler = Butler()
        time.sleep(2)
        while True:
            us = butler.get_ultrasonics()
            print(us)
            if(min(us) < 501):
                butler.stop()
                butler.set_leds(butler.YELLOW)
                print("Waiting 3 seconds\n")
                start_time = monotonic()
                while min(us) < 501 and monotonic() - start_time <= 3.0:
                    us = butler.get_ultrasonics()
                    print(us)
                    sleep(0.2)
                if min(us) > 500:
                    print("Obstacle gone, resuming")
                else: # Persistent obstacle, turn left 5 times
                    n = 0
                    while n < 5 and min(us) < 501:
                        n += 1
                        butler.turn_left()
                        time.sleep(1.5)
                        us = butler.get_ultrasonics()
                    if min(us) > 500:
                        print("sObstacle avoided, resuming")
                    else:
                        butler.set_leds(butler.RED)
                        input("Press enter to resume")
            else: # no obstacles
                butler.forward()
                butler.set_leds(butler.GREEN)
            sleep(0.2)
    except KeyboardInterrupt:
        butler.stop()
        butler.set_leds(butler.OFF)