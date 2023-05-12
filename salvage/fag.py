import time
import sys,tty,termios,os
import RPi.GPIO as GPIO
from SunFounder_PCA9685 import Servo
DIR1 = 24
DIR2 = 23
GPIO.setmode(GPIO.BOARD)
GPIO.setup(DIR1,GPIO.OUT)
GPIO.setup(DIR2,GPIO.OUT)
myservo = []
myservo.append(Servo.Servo(0))  # channel 1
myservo.append(Servo.Servo(1))  # channel 1
Servo.Servo(0).setup()
Servo.Servo(1).setup()
print("DubbiDubbiDub")
		
#Steering instructions
print("w/s: forward and backward")
print("a/d: left and right steering")
print("x: Stop motors")
print("g: to enter autonomous mode")
print("q: Quit program")


def forward():
	time.sleep(0.001)
	GPIO.output(DIR2,0)
	GPIO.output(DIR1,0)
	myservo[0].write(30)
	myservo[1].write(30)
	time.sleep(0.001)

def reverse():
	time.sleep(0.001)
	GPIO.output(DIR2,1)
	GPIO.output(DIR1,1)
	myservo[0].write(32)
	myservo[1].write(30)
	time.sleep(0.001)

def left():
	GPIO.output(DIR2,1)
	GPIO.output(DIR1,0)
	time.sleep(0.001)
	myservo[0].write(30)
	myservo[1].write(30)
	time.sleep(0.001)

def right():
	GPIO.output(DIR2,0)
	GPIO.output(DIR1,1)
	time.sleep(0.001)
	myservo[0].write(30)
	myservo[1].write(30)
	time.sleep(0.001)

def stop():
	time.sleep(0.001)
	myservo[0].write(180)
	myservo[1].write(180)
	time.sleep(0.001)

def blinkDirs():
	GPIO.output(DIR2,0)
	GPIO.output(DIR1,0)
	time.sleep(1)
	GPIO.output(DIR1,1)
	GPIO.output(DIR2,1)
	time.sleep(1)
	

# The catch method determines which key the user pressed on the keyboard
def getch():
	fd=sys.stdin.fileno()
	old_settings = termios.tcgetattr(fd)
	try:
		tty.setraw(sys.stdin.fileno())
		ch = sys.stdin.read(1)
	finally:
		termios.tcsetattr(fd,termios.TCSADRAIN,old_settings)
	
	return ch

while True:
	char = getch()
	if(char == "w"):
		forward()
		time.sleep(0.7)
		stop()
		char = ""
		print("W")
	if(char == "s"):
		reverse()
		time.sleep(0.7)
		stop()
		char = ""
		print("W")
	
	elif(char == "r"):
		stop()
		time.sleep(0.7)
		char = ""
		print("s")
	elif(char == "a"):
		left()
		time.sleep(0.1)
		stop()
		char = ""
		print("a")
	elif(char == "d"):
		right()
		time.sleep(0.1)
		stop()
		char = ""
		print("a")
	elif(char == "y"):
		blinkDirs()
		blinkDirs()	
		char = ""
		print("y")
	elif(char == "q"):
		stop()
		char = ""
		print("Program Ended")

		break

	char = ""



	
