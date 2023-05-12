# Hardware Interface
To interface with the motor controllers and sensors, a real time processor is
needed. For this purpose an Arduino Uno is used.

## Hardware
The interface controls the following things:
- Dual motor controller
- 2x Incremental Quadrature Encoders
- 3x Ultrasonic range sensors
- RGB LED Strip driver

The needed PWM signals are generated using a PCA9685 breakout board connected
over I2C.

## Pin Assignments
The following list is only for reference, you should refer to the definitions in
the relevant header files.  
- **D2 (PD2):** Encoder 1 B input
- **D3 (PD3):** Encoder 1 A input
- **D4 (PD4):** Encoder 2 A input
- **D5 (PD5):** Encoder 2 B input
- **D6 (PD6):** Motor 1 direction output
- **D7 (PD7):** Motor 2 direction output
- **D8 (PB0):** Diode OR-ed ultrasonic ECHO input
- **D9 (PB1):** Ultrasonic sensor 1 TRIG output
- **D10 (PB2):** Ultrasonic sensor 2 TRIG output
- **D11 (PB3):** Ultrasonic sensor 3 TRIG output
- **D18/SDA (PC4):** PCA9685 I2C bus SDA
- **D19/SCL (PC5):** PCA9685 I2C bus SCL

Additionally, the following channels of the PC9685 are used:
- **Ch. 0:** Motor 1 PWM output
- **Ch. 1:** Motor 2 PWM output
- **Ch. 4:** LED Strip Green channel
- **Ch. 5:** LED Strip Red channel
- **Ch. 6:** LED Strip Blue channel

## Serial
The Arduino acts as the intermediary between the hardware and a Raspberry Pi. It
receives commands from the Pi over a serial connection, processes these
commands, and sends back relevant sensor data. 

Commands follow a simple structure: a single-letter command identifier, followed
by between zero and three integer arguments, depending on the command.

The serial connection should be made at 115200 baud with 8,N,1 settings. Every command must be ended with a carriage return (ascii 13, python '\r'), however line feed characters are ignored, so sending CR LF is also fine.

## Encoders
The encoders are polled regularly via a timer interrupt. Their pin states are
used to update the global encoder values, which are stored in 32-bit signed
integers. Their states can be retrieved with the 'e' command.  
**Example:** 
```
e
{5067 413}
```

## PWM (Motor controller and PWM Output)
The PCA9685 breakout board is used to generate PWM signals. It is controlled
using Adafruit's PCA9685 library, which is included as a git submodule.  
If the Arduino IDE complains that it can't find it, you probably have to run
`git submodule update --init` to fetch the library.

PWM duty cycles can be set with the 'o' command. Its first argument is the
channel you want to set the duty cycle of, and the second argument is the new
duty cycle as a 12-bit value (i.e. an integer between 0 and 4095 inclusive).

**Example:** 
```
o 0 2048
OK
```
This command sets channel 0 to a duty cycle of 2048 (50%).

## Motors
The 'm' command can be used to set the direction and PWM duty cycle of both
motors simultaneously. It sets the direction pins based on the sign of the
arguments, which should be in the range -4095 to 4095 inclusive, and uses
channels 0 and 1 of the PCA9685 to generate the PWM signals for the motor
controller.
**Example:**
```
m -600 600
OK
```
This command makes the robot turn left rapidly.

## Ultrasonic Sensors
The ultrasonic sensors are read one after another at 60ms intervals, to prevent
the echos from interfering with each other. This is accomplished by using a
state machine driven by the main loop to operate the input capture peripheral of
the ATMega328P.  
The 'e' command can be used to get the latest stored value for all three
sensors. In case a sensor becomes disconnected, a value of -1 will be returned
for it. If a sensor times out waiting for an echo (i.e. nothing is in range) a
value of 5000 will be returned for it.

**Example:** 
```
u
{5000 256 -1}
```
In this case the right sensor (sensor 1) sees no obstacles within range, the
center sensor reads a range of 25.6 centimeters, and the left sensor is
experiencing a fault.
