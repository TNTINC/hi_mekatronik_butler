# 2023-05-12
Merged this documentation and the butler_hardware_interface repository (the arduino code) into one.

Finished first draft of report.

# 2023-05-08
Drilled the hole in the platter and mounted it with the 3d-printed parts.

# 2023-05-05
Full assembly and driving, nearly ran over Kalli.

Tried to get motor PID to work, but the encoders are just too coarse, it
    doesn't work at all and I can't make it work.

Mounted the LEDs, they look good.

The ultrasonic sensors have an issue. When nothing is in range, instead of timing out, they simply hang indefinitely.
    Sending another trigger signal doesn't appear to do anything.
    Driving echo low doesn't appear to do anything.
    Cutting power to the sensor only works for some of the sensors.
        Certain models hold ECHO high for ~60ms on startup.
    I've managed to scrounge up 3 sensors from around the lab that work.

Wrote some basic code to make it drive around and avoid obstacles.

# 2023-05-04
Made and mounted a connector board for the ultrasonic cable, and wired it up.

Connected the electronics assembly to a 12v bench supply and turned it on.
    No fire, current consumption about 200ma. All good.

Connected all the ultrasonics and discovered that the Echo line needs diodes
in order for the sensors to not interfere with eachother.
    Connected the sensors with jumper wires and 5.1v zener diodes for now.

Connected a motor and encoder to the electronics assembly and tested it.
    All works.

Getting the raspberry pi set up to program the arduino.
    Compiling python for the raspberry pi.



# 2023-05-03
Mounted the reprinted encoder mounts to the motors.

Went out to the shop with Kalli and looked for a holesaw unsuccessfully. 
    He's taken the platter and said he'll bring a set from home tomorrow.
    We settled on mounting the plate right on top of the pipe (~90cm from gnd).

Cut the electronics plate down a bit and mounted the pipe clamps to it. 
    Fits very well onto the pipe.

Mounted the fuse holder into the chassis next to the on/off switch.

I need some sort of power distribution board.

Made a power distribution board.
    Just some parallel terminal blocks and connectors for the encoders.
    I should document it a little better.

Worked on assembling the electronics plate.
    Mounted the pi, arduino, pca, motor driver, led driver and power 
    distribution board.
    Wired everything up with jumpers.
    Made and installed all the fixed 12v and gnd wires.
    I should make a proper block and wiring diagram.

Made a custom cable to connect all three ultrasonic sensors to a 6-pin header.
    This took forever, crimping tiny ribbon cables is awful.

Returned the Arduino nano, the ultrasonic, the pi, the usb cable, the mosfets.

Goals for tomorrow: It driving around.

# 2023-05-2
Encoder mounts are ~1.5mm too tall.

Started charging battery 1 using Optimate ampmatic 6.
    Battery 1 charged in only an hour and passed the charger's testing.
    Started charging battery 2. Done.

Disassembled the E-stop to take measurements.

Procured wiring supplies, fuses and fuseholder, and terminal blocks for LEDs.
    1 and 1.5q wires and ferrules from the storage locker in the 3rd floor lab
    Bag of push in terminal blocks from dingy 3rd floot storage room

Temporarily mounted encoder to motor.

With motor at full speed the A phase has a frequency of only 80Hz.
    This will likely make measuring velocity quite difficult.
    At the lowest maintainable velocity (1.2v~) the frequency is less than 10hz.

Connected motor driver, pca, arduino together and spun the motors. Works well.

Finished soldering up the LED Driver.

Measured the platter, it's 378mm OD and 333mm ID with a 25mm step, 1mm thick.

Reprinted the motor mounts, and designed and printed some pipe clamps to mount 
    the electronics plate to the pipe.

# 2023-05-1
Mostly took the day off, ended up working on the LED driver board.

# 2023-04-30
Working on using the PCA9685 to generate PWM signal.
    Choosing to go with the adafruit library since it's probably the most mature.
    Got that working without incident.

Plan was to work on motor PID control, but I can't really do that without having the motor controller and encoders attached to the real motor.

Instead I'll work on the ultrasonic sensors.    
    Plan is to use the timer1 input capture peripheral to get the pulse lengths, however there's only one of those so the sensors will have to share.
    not very useful datasheet: https://cdn.sparkfun.com/datasheets/Sensors/Proximity/HCSR04.pdf
    Plan succeeded, ended up making a little state machine.


# 2023-04-29
Designed and printed the encoder mounts and soldered some headers onto the encoders.

Made a start on the arduino code, got a serial command interface working and managed to read an encoder.

# 2023-04-28
Busy with life, managed to get the Pi serial terminal working.
Reset the Pi password to raspberry

# 2023-04-27
Mounted the sd card that was in the RPi 3b (15Gb sandisk) and explored it.
    Found "andrith@hi.is" eduroam login credentials in wpa_supplicant.conf
    Found some code at /home/pi/version1/fag.py
    The code doesn't seem to be the final version, very short and with no means
    to read the ultrasonic sensors, I wonder where that code ended up?

Battery health
    The batteries are CP12120 12V 12AH Sealed led acid packs.
    Manual: https://vision-batt.eu/sites/default/files/public/docs/products/manuals/CP12120.pdf
    Their design life is 5 years, last known use was in 2017.
    OCV pack 1: 12.5V, pack 2: 12.4V.
    Presumably need charging, TODO: ask Andri and Kalli about it.

System architecture seems to have been the Pi driving the motors via a PCA9685
 PWM breakout, and reading the ultrasonic sensors directly using the python 
 GPIO library. 

The custom circuit board seems to be in rough shape. The motor power supplies
 are fused on it, but only one fuse is installed. There is a cold
 solder joint on one of the motor terminal blocks.

 Current thoughts:
    I had assumed getting it to drive would be straightforward but this doesn't
     seem to be the case. Adding encoders to allow it to drive slowly will
     require the addition of a microcontroller to read them.
    I should try it without encoders first to see how it performs.
Microcontroller selection: Done.
    Read quadrature encoders quickly and reliably.
    Read ultrasonic sensor pulses reliably.
    Easy to program and familiar to students.
        means Arduino IDE or MPLab, no platformio.
    Existing code for ATMega32u4.
    Arduino nano (328p) provided by Kalli.
    
Goals:
   - Drive
       - Power
           - New power distribution board?
       - Software
           - Low level
           - High level libraries
           - High level behavior
       - Motor controller
       - Encoders
           - Physical mounting
           - Reading (external micro)
   - Sense
       - Ultrasonic sensors
   - Light up
        - LED Driver board
           - Three mosfets
           - Piece of vero board
           - Connectors
   - Speak
       - Old speaker
   - Relocate platter to top
       - Good height would be 100cm off the ground or 96cm from robot base.
       - Existing central tube is 40mm dia and 85 from robot base,
            with a 70mm section at the top turned down to 38mm (under by 0.15)
   - Be documented and extensible by future students
       - Figure out way to persist knowledge
           - Wiki system seems too fragile
           - Github repo
               - Ownership transfer/access rights
               - Just fork it every time? Not good for links
               - Requires technical skill
           - Bunch of files on a USB stick
               - Reliable, not discoverable, not very convenient to edit
           - Folder on department onedrive
               - QR code link on inside of robot
               - TODO: Ask a&k about feasibility of this
       - Write behavioral software in python
           - Module to deal with the ultrasonics
           - Module to deal with driving the motors, maybe differentially

Tasks in no particular order:
- Charge batteries
- Solder headers to motor driver
- Get solid access to the Pi on the eduroam network (HDMI display? SSH?)
- Encoder mount
   - Take measurements
   - CAD and 3d print
   - Install and test
- LED Driver board
