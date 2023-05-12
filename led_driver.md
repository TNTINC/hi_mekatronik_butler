
# LED Strips and driver

In the robot there are 2 meters of 12V RGB LED strip.
Details of the strips:
    Digikey Pn.: 1568-1153-ND
    Link: https://www.digikey.com/en/products/detail/sparkfun-electronics/COM-12021/5673739
    Mfr.: Sparkfun
    Mfr.Pn.: COM-12021
    MFr. Link: https://www.sparkfun.com/products/12021
    Original packaging states 14.4W/meter power draw.

These strips are non-addressable and common-anode, meaning a driver is needed.
The strips have inbuilt resistors for each color, so the driver circuit is 
simply three discrete N-channel MOSFETs, one for each color.

## Choosing mosfets
Since the rated power draw of the strips is 14.4W/meter, the total power is 
28.8W for the 2 meters. Each mosfet needs to handle one third of that, so 
around 10W. The mosfets that were on hand are IRFZ44N's 
    (Datasheet: https://www.infineon.com/dgdl/Infineon-IRFZ44N-DataSheet-v01_01-EN.pdf?fileId=5546d462533600a40153563b3a9f220d).
To calculate whether they can handle the current we find the power dissipated in
them due to their on-resistance (Rds(on)), then estimate the temperature rise 
due to that power dissipation, and check that it is within limits.
In more complex cases it's necessary to account for switching losses, but in 
this case the frequency is low enough that they can be ignored.
The relevant values from the datasheet are:
- Rds(on) @ Vgs=10V: 0.0175 Ohm 
- Thermal resistance, junction to ambient: 62 °C/W
- Maximum operating temperature: 175 °C
10W at 12V is 0.83A, so the power dissipated in the mosfet is 
    0.83^2 * 0.0175 = 0.012W.
The resulting temperature rise is 0.012 * 62 = 0.74 °C, which is utterly negligible.
Even if the on-resistance at 5V were 100x higher, the temperature rise would 
only be 74 °C.
So, the mosfets will be absolutely fine.

The total input current at full brightness white would be 28.8W/12V = 2.4A, 
which is just barely within the specifications for a dupont (jumper wire) connector (2.5A). Likely we'll only ever have two colors at full brightness at 
once, so the actual current draw will be less than that.
Still, let's use  terminal blocks.

## Circuit
The schematic of the driver was drawn in KiCad and the project files are 
available in the "led_driver" directory. It has a 6 position terminal block and 
a three position header strip.

Pin 1 on the terminal block is marked with a black square, this is ground.
Pin 2 is 12V in. Pins 3, 4, 5, and 6 correspond to the black, blue, red, and 
green wires of the LED strips.
The header strip's pin 1 is marked with a black triangle, and it controls the 
green channel. Pin 2 controls red, and pin 3 controls blue.