
# User account
The default user account is used, 'pi' with password 'raspberry'.

# Serial console
The usual way to access a Raspberry Pi is via SSH, however since the Eduroam wifi doesn't allow 
static addresses to be assigned, it's not possible to know the IP address of the Pi to connect to.
So instead we use the serial console.
The serial console is available on pins 8(IO14) and 10(IO15) and ground on 6 (see: https://pinout.xyz/pinout/uart#).
It expects 115200 baud, 8N1 at 3.3V levels. Be careful not to connect a 5V serial adapter to it.
You can look at https://learn.adafruit.com/adafruits-raspberry-pi-lesson-5-using-a-console-cable
for more information on how to connect to the serial console.

Once logged in via the serial console you can start developing using tmux, vim etc. if you're into 
that kind of thing, or you can connect it to a wifi network and use `ip addr show` to find its IP.
To connect it to WiFi refer to: https://www.raspberrypi.com/documentation/computers/configuration.html#wireless-networking-command-line
Note that connecting to eduroam is not possible with this method, but can be done by manually 
editing /etc/wpa_supplicant/wpa_supplicant.conf as per https://medium.com/@celikemirhan/how-can-you-connect-eduroam-wi-fi-with-raspberry-pi-8f704e6fa7f6
However I recommend just running a phone hotspot and connecting to that.

# Software
in /home/pi/butler_behaviour is the code for the butler behaviour. It currently consists only of 
the file test.py, which simply makes the robot drive forward until an obstacle is detected, then 
turn left and repeat.
It's written in python 3, and uses the pyserial library to communicate with the hardware interface.
On the raspberry pi, `python` points to python 2, so you have to use `python3 test.py` to run it.
Hopefully it's straightforward enough to provide some insight into how to interface with the robot.