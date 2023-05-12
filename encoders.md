To accurately control the speed of the wheels, encoders are needed.
The chosen encoders are Pololu item #1523, 12 count/rev magnetic encoders.
See: https://www.pololu.com/product/1523

# Software
Bit width to store encoder values:
    12 Count/Rev encoders
    Assuming continuous rotation at 60 RPM,
        signed 16bit (2^15) would last 45 minutes
        signed 24bit (2^23) would last 8 days
        signed 32bit (2^31) would last 5.6 years

# Mounting
The encoder discs (magnets) are attached to the motor shafts with superglue.
If they need to be removed, soaking the joint with acetone will dissolve it.

To mount the encoders to the motor, 3D printed encoder mounts were designed.  
The encoder mounts are attached to the face of the motor mounting brackets with
M3x25 screws. The encoders themselves slide into recesses in the mounts, and are
secured with superglue.    