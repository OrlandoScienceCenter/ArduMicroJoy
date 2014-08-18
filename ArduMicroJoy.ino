

/*
Arduino Leonardo Joystick!

*/

// Analog Input Defines
#define XAXISPIN     A0
#define YAXISPIN     A1
#define RUDDPIN      A2
#define THROTPIN     A3

// Joystick Physical Button Pin Defines
#define JOYPIN1       6
#define JOYPIN2      18
#define JOYPIN3      10  
#define JOYPIN4      12

// General Button Defines
#define MODEPINAUTO   5
#define MODEPINMAN    7
#define SIMRESETPIN   9  



// General Defines
#define USBDELAY      5
#define LED7SEG_VCC   4



// NOTE: PINS 2 AND 3 ARE USED BY SDA/SCL AND CANNOT BE USED

//INCLUDES
#include <Wire.h>                  //I2C Library
#include <Adafruit_LEDBackpack.h>  //Adafruit Backpack Libraries
#include <Adafruit_GFX.h>



//LIBRARY ATTACHMENTS
Adafruit_7segment LEDmatrix = Adafruit_7segment();


//GLOBAL VARS





JoyState_t joySt;

void setup()
{
	pinMode(13, OUTPUT);

	joySt.xAxis = 0;
	joySt.yAxis = 0;
	joySt.throttle = 100;
        joySt.rudder = 100;
	joySt.buttons = 0;

LEDmatrix.begin(0x70);

}


void loop()
{
	joySt.xAxis = random(1023);
	joySt.yAxis = random(1023);
	//joySt.zAxis = random(1023);
	//joySt.throttle = random(1023);
	

//        joySt.throttle++;
//	joySt.rudder++;

	joySt.buttons <<= 1;
	if (joySt.buttons == 0)
		joySt.buttons = 1;

	delay(USBDELAY);

	Joystick.setState(&joySt);    // Send that data bits 

}


void readAnalogControls()
{
  // Reads the analog input at full 10bit, and sets joystick variables (16bit) directly
  joySt.xAxis = analogRead(XAXISPIN);
  joySt.yAxis = analogRead(YAXISPIN);
  joySt.rudder = analogRead(RUDDPIN);
  joySt.throttle = analogRead(THROTPIN);

}

void readButtonStates ()
{
  // read the pins and record states
  // bitshift into joySt.button
}

void readModeState ()

{
  // read and set flags for loop
  // possibly integrate into main loop instead
}

void readRemoteButtons ()
{
  // read the button /remote controls
  // looking at using pins 12/13 for the input. so. many. pins.  
}

void turnComputerOn ()

{
 // Self explanitory 
}

void turnComputerOff ()

{
  //Self explanitory
}
