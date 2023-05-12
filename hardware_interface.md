# Hardware Interface
To interface with the motor controllers and sensors, a real time processor is needed.
For this purpose an Arduino Uno is chosen.

## Hardware
The interface controls the following things:
- Dual motor controller
- 2x Encoders
- 3x Ultrasonic range sensors
- RGB LED Strip driver

The needed PWM signals are generated using a PCA9685 breakout board connected over I2C.

## Software
The Arduino receives commands from the Raspberry Pi over serial, and sends back sensor data.
All commands are a single letter followed by 0-3 integer arguments.

### Encoders
A regular timer interrupt is used to poll the encoder pins, and update the global encoder values by using the last and current pin states to index into a state transition lookup table.

The encoder values are stored in 32bit signed integers, which is enough to store 5.6 years of continuous rotation at 60 RPM.
They can be read using the 'e' command.

### PWM (Motor controller, LED driver)
The PCA9685 breakout board is used to generate PWM signals.
Adafruit's PCA9685 library is used to control it.