# gamepad_adk
A library for interfacing most USB gamepads with Arduino
### Usage
In the header section of main code, add the following:
...
#include <usbhid.h>
#include <hiduniversal.h>
#include <usbhub.h>

#ifdef dobogusinclude
#include <spi4teensy3.h>
#include <SPI.h>
#endif

#include "usbremote.h"

bool debug_flag = false;
int one=0, two=0, three=0, four=0, up=0, down=0, left=0, right=0, l1=0, l2=0, l3=0, r1=0, r2=0, r3=0, x=0, y=0, z1=0, z2=0, rz=0, start=0, sel;

USB Usb;
USBHub Hub(&Usb);
HIDUniversal Hid(&Usb);
JoystickEvents JoyEvents;
JoystickReportParser Joy(&JoyEvents, debug_flag, &one, &two, &three, &four, &up, &down, &left, &right, &l1, &l2, &l3, &r1, &r2, &r3, &x, &y, &z1, &z2, &rz, &start, &sel);
...

In the void setup() function,

...
Serial.begin(115200);
if (Usb.Init() == -1)
    Serial.println("OSC did not start.");
delay(200);
if (!Hid.SetReportParser(0, &Joy))
    ErrorMessage<uint8_t > (PSTR("SetReportParser"), 1);
...

Whenever the remote values need to be updated, call Usb.Task()

The code is based on [USB Host Shield 2.0 Library][https://github.com/felis/USB_Host_Shield_2.0] by [felis][https://github.com/felis/]

...
&copy; [Nabarun Pal][http://nabarun.in] 2016 GNU GPL v3
...
