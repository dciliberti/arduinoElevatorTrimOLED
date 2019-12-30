# FlightGear Analog Trim Tab Wheel
I wanted an additional analog control in FlightGear - nice, cheap and compact.

## Task
Connecting Arduino to FlightGear is nothing new, but I've found that wikis are outdated and forums give precious hints where you have to work around.

The idea was to have a nice, cheap and compact analog command to control the elevator trim tab wheel of an aircraft in FlightGear. The wiki works until the communication part. Strictly speaking, you cannot directly use serial communication to pass commands from Arduino to FlightGear. You have to workaround with Python to send packets via UDP.

What is a trim wheel? It is an analog device to control the deflection of the trim tab, which is an aerodynamic device to keep a control surface (in this case the elevator controlling the aircraft pitch) in a given position without effort from the pilot. From [Wikipedia](https://en.wikipedia.org/wiki/Trim_tab):

>Trim tabs are small surfaces connected to the trailing edge of a larger control surface on a boat or aircraft, used to control the trim of the controls, i.e. to counteract hydro- or aerodynamic forces and stabilise the boat or aircraft in a particular desired attitude without the need for the operator to constantly apply a control force. This is done by adjusting the angle of the tab relative to the larger surface.

## Requirements
Additional requirements for this project: python with the libraries serial, socket, and sys installed. Assuming that Anaconda has been installed, the missing libraries can be retrieved with [Anaconda](https://www.anaconda.com/distribution/) prompt by typing:

`>>> conda install -c anaconda pyserial`

the same with serial and socket. Search on the internet for an appropriate package, usually the most downloaded for your platform are good. Once you have python with these libraries (and, of course, [FlightGear](https://www.flightgear.org/) installed) you may move to the next section.

There are three code scripts: an Arduino sketch, a python executable script, and an XML script. The Arduino sketch must be loaded on your board. The python script must be executed to enable communication between Arduino and FlightGear. The XML script is a FlightGear generic protocol that must be moved to `<FlightGearRoot>/data/Protocol` once for all.

The Arduino sketch requires the [U8glib library](https://github.com/olikraus/u8glib). As mentioned in the comment, adjust the sketch's line 2 with your display:

```
#include <U8glib.h>
U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE|U8G_I2C_OPT_DEV_0);  // I2C / TWI
```

See U8glib examples and documentation. The sketch should work even without a display, so you may play without visual feedback from your board.

## Notes
If you are unhappy with direction of rotation, just switch the position of the last two arguments in the following map instructions

```
w = map(p,0,1023,100,-100);
m = map(p,0,1023,90,0);
```

so that the values of the potentiometer go from -100 to 100 and the needle's gauge goes from 0 to 90 (this is a trick to draw the needle in the correct position, try yourself if you are curious).

The python script includes the host address and the serial port number. The host should be `"localhost"` (if it doesn't work, try with 127.0.0.1), the port any (maybe it is better one not in use by other applications, I don't know), the serial port where your board is connected may be read from the Arduino IDE. In my case, it is `"COM4"`. **If you test the Arduino with the serial monitor, you may have to disconnect and re-connect your board before executing the python script in order tomakeit work properly.**

```
host = "localhost"
port = 21567
#buf = 1024
addr = (host,port)
UDPSock = socket(AF_INET,SOCK_DGRAM)
tty="COM4"
print("tty is ", tty)
```

The python script may be located anywhere. You may execute it from the command line of your operative system. In my case (Windows), I type in the command prompt:

`python c:\path\to\file\arduinoElevatorTrim.py`

This may be launched before or even after starting FlightGear.

Finally, as previously stated, the XML script must be moved once for all in the protocol folder of your FlightGear installation. Now you are ready to go. Launch FlightGear with your preferred mode (command line or launcher) by adding the following command **with no space between chars!**:

`--generic=socket,in,30,localhost,21567,udp,arduinoElevatorTrim`

where we are telling FlightGear to apply a generic protocol via socket, read input 30 times per second, on the address localhost, port 21567, UDP protocol, instructions in the arduinoElevatorTrim.xml file. Enjoy your personal trim wheel!

### arduinoElevatorTrimOLED.ino
This is the script to load on the Arduino board. Adjust line 16 (type of display) as needed.

### arduinoElevatorTrim.py
This is the script to be executed along with FlightGear to "translate" serial communication into UDP packets. You need Python installed in order to execute this script. Modify line 14 (USB port where the Arduino is connected) as needed.

### arduinoElevatorTrim.xml
this is the script to add in the folder <FlightGearRoot>/data/Protocol.

## External sources
[Oled gauge graphics by pakganern](https://steemit.com/utopian-io/@pakganern/oled-display-gauge-meter-using-potentiometer-arduino)

[This device on the Arduino Project Hub](https://create.arduino.cc/projecthub/dancili/flightgear-analog-trim-tab-wheel-f104b8?ref=user&ref_id=238025&offset=1)

[Video demonstration](https://youtu.be/hvd_2HRwMwk)
