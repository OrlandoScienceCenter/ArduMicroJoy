Arduino Micro Joystick Control - Designed for OSC Exhibits

Use the included arduino 1.6.3 to modify the code. It has a modified USB.cpp and USBHID.h in the cores folder that allows the micro to behave as a joystick

Required Libraries: None

Creates a joystick control for 4 analog and 16 button inputs using an arduino
micro. 


Old instructions: Not necessary if using the included IDE:

	Place the hardware directory inside of your arduino program directory - this will overwrite two files needed to change the HID descriptor files and setup the proper variables. Make a backup of this drectory before you do this! - call it something like arduino-standard so you can rever easy. 

Created by Hybridsix (Michael King)
