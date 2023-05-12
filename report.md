
# Work completed
## Initial Assessment
The robot was located in the robotics lab on the second floor.  
An initial visual assessment showed that key components had been removed and it
was not in an operable state.

Stakeholders were asked about prior work relating to the robot which only turned
up a BS thesis from 2017 (Available: <http://hdl.handle.net/1946/27771>).  
At the conclusion of that thesis the robot had functioning manual and autonomous
modes of operation including obstacle avoidance.

Removing the outer shell of the robot revealed the electronics package: an
acrylic plate mounted with twisted wire to the central column. On this plate
were a Raspberry Pi 3 computer, a 5V UBEC spliced to a micro-USB cable powering
the raspberry pi, a PC9685 pwm generator, and a custom PCB.  
The only connections were some jumpers connecting the RPi and PCA9685, and very
long jumpers connecting the three ultrasonic sensors to the custom pcb. No motor
drivers were installed, and the PCB did not match anything from the 2017 thesis.

The filesystem of the Raspberry Pi was explored but only 3rd party libraries and
one unfinished python script were found in the home directory. My initial
conclusion was thus that the previously written code had somehow been lost, and
work proceeded on that basis.

However, while writing this report and reviewing the 2017 thesis in more detail
I discovered that the code was likely located in a less obvious directory that I
hadn't searched. More importantly, one of the thesis' appendices included a
complete code listing. That this was missed originally is unfortunate and
highlights the importance of thorough literature review, but having reviewed it
it ultimately would not have greatly affected the course of my work.

Beyond the electronics package, the two 12v sealed lead acid batteries were
found mounted to the chassis although without tightened nuts securing them down.
Their open-circuit voltages were measured and found to be nominal, and they
appeared to be in good condition.

The motors, belts, pulleys and wheels were all present and securely mounted, and
appeared to be in good condition. Turning the wheels by hand did indicate that
the belt tension was less than optimal and that the pulley alignment could be
improved, but it was within useable limits.

## Design and planning

Brainstorming took place shortly after the project kickoff meeting, and resulted
in several possible goals. Their feasibility was explored and they were
categorized into two possible levels of functionality which were presented to
faculty.

The first level of functionality consisted of three primary objectives:

1. **Autonomous Driving:** The robot was expected to navigate autonomously
   within a crowded room without hitting any obstacles, using the sensors
   already installed in it.

2. **Illumination:** The robot was expected to light up in different colors
   through its translucent shell. This was designed to make the robot more
   visually appealing and noticeable in a crowded room.

3. **Verbal Interaction:** When encountering an obstacle, the robot should be
   able to verbally communicate by saying "Pardon me". This feature was aimed at
   making the robot more interactive and polite, creating a friendly atmosphere
   during its operation.

The second level of functionality aimed to extend the robot's capabilities, including:

1. **Path Navigation:** The robot was expected to navigate a preset path that
   spanned multiple rooms. This would allow the robot to serve multiple areas
   during an event without requiring manual control. This function required the
   development of a more sophisticated navigation system, possibly using a
   combination of more expensive onboard sensors and external reference points.

2. **Drink Detection and Return to Base:** The robot was also expected to be
   able to detect when drinks had been removed from its platter and then return
   to a designated home base. This feature was aimed at making the robot more
   self-sufficient and reducing the need for human intervention. This required
   the development of a weight or presence detection system for the drinks, as
   well as a method for the robot to identify and navigate to its home base.

Ultimately it was agreed to pursue only the first level of functionality, so a
detailed bill of materials for it was prepared. Unfortunately, supplier issues
caused a considerable delay in the delivery of the necessary parts

## Implementation 

To achieve the set goals the following things were done:

1. **Hardware:** Modifications and additions were made to the robot's mechanical and electrical components:
    - Encoders were mounted to the motors using 3D-printed encoder mounts.
    - An Arduino Uno microcontroller was added to the hardware architecture to
      read the encoders and handle timing critical functions of the robot, and
      permit full utilization of the Raspberry Pi in the future. It communicates
      with the RPi via 115200 baud serial over USB.
    - A dual motor driver from Pololu was procured and wired to the motors.
    - An LED driver board was designed and fabricated and an LED strip was
      mounted inside the robot.
    - A power distribution board was made to simplify the wiring of the robot.
    - The electronics plate was modified to hold the Raspberry Pi, Arduino,
      PCA9685, motor driver, LED driver, and power distribution board. Pipe
      clamps were 3D-printed and used to securely attach it to the central
      column of the robot.
    - 3D printed parts were designed to allow mounting of the platter on top of
      the robot and modifications to the platter were made to accommodate the
      new design.

2. **Software Development:** Code was written for the Arduino and Raspberry Pi:
    - A serial command parser was written for the Arduino.
    - 
    - Python code to interface with the arduino and drive around while avoiding
      obstacles was written for the Raspberry Pi.
- 

1. **Testing and Debugging:** Throughout this process, testing was conducted to ensure that the robot's components were functioning correctly. Several issues were identified and addressed:
    - A motor and encoder were connected to the electronics assembly for testing.
    - A problem was identified with the ultrasonic sensors: they did not timeout and hung indefinitely when nothing was in range. This was partially solved by finding alternative sensors that worked correctly.
    - The robot was assembled and driven, which almost resulted in an accident with a team member, highlighting the need for careful testing.

## Final State (May 8, 2023)

By this date, the robot had been fully assembled with all its components, including the modified platter, 3D-printed parts, ultrasonic sensors, LED lights, motor, and encoders. The final test demonstrated that the robot could move around and avoid obstacles. However, a PID motor control system could not be implemented due to the encoders' coarse readings. Despite this, the robot was in a functional state ready to serve its purpose at university events.


- A main fuse was added.
- A motor driver was integrated.
- A microcontroller was integrated and programmed.
- An LED strip was installed and an LED strip driver was made.
- The platter was relocated from the side of the robot to the top.
- Basic code was written to allow the robot to drive around autonomously.

## Current state

# Guide for Use
To turn on the robot, first ensure that the top emergency stop is reset, then
find the power switch on the bottom of the robot at the back, slightly right of
center. Press it towards the center to turn everything on. Allow about 1 minute
for the raspberry pi to fully boot.

As of now only a single test script has been written for the robot,
which makes it drive around and turn left whenever it detects an obstacle. To
start this script, first connect to the raspberry pi via serial or ssh. For
instructions on that see [raspberry_pi.md](./raspberry_pi.md).  
Once connected navigate to `/home/pi/butler_behaviour/` and run
```bash
pi@raspberrypi:~/butler_behaviour $ python3 test.py
```
To stop the robot press Ctrl-C, or hit the emergency stop.

To disassemble the robot for access, first the platter must be removed. It is
held in place with three M3 screws from the top, which once removed allow it to
be lifted off of its mounting flange. The dome can then be lifted off similarly.  
The main shell is fastened in three places at the bottom, once
those fasteners are removed it too can be lifted off the chassis.

# Suggested Actions
1. Properly secure the 12v batteries, preferably with wingnuts.
2. Tidy up the wiring of the ultrasonic sensors.
3. Properly align and tension the drive belts.
4. Purchase and install higher resolution encoders such as [AMT102-V][encoders],
   and implement proper PID speed control of the motors to allow the robot to
   drive straight and at a consistent speed.  
5. Write more sophisticated autonomous behaviour code.

[encoders]: <https://www.digikey.com/en/products/detail/cui-devices/AMT102-V/827015>

# Possible Improvements
Install usb-b port on outside of robot for convenient serial terminal access.