
/*
Arduino Micro Joystick Control - Designed for OSC Exhibits

Required Libraries Adafruit_GFX Adafruit_LEDBackpack

Creates a joystick control for 4 analog and 16 button inputs using an arduino micro. Also has provisions for 7 Segment timer display and reset, along with RF Remote Button control for system on/off.

Created by Hybridsix (Michael King)
*/


/***********************************************************
*                         DEFINES                          *
***********************************************************/

// Analog Input Defines
#define XAXISPIN           A0
#define YAXISPIN           A1
#define RUDDPIN            A2
#define THROTPIN           A3

// Joystick Physical Button Pin Defines
#define JOYPIN1             6
#define JOYPIN2            18
#define JOYPIN3            10  
#define JOYPIN4            12      // Duplicaated address for REMOTE_ON - both will turn on the system if it is off. 

// General PIN and Button Defines
#define MODEPINAUTO         5
#define MODEPINMAN          7
#define SIMRESETPIN         9  
#define REMOTEIN_ON         12       // RF Remote input -  Pin D3(A) from RF - ON control ---- Duplicaated as define JOYPIN4 
#define REMOTEIN_OFF        13       // RF Remote input -  Pin D0(D) from RF - OFF control
#define COMPUTER_PWR        11       // Computer power button pin - ground for press
#define COMPUTER_SENSE      A5       // Computer power LED pin - tied to +5v of computer PWR LED pin



// General Defines
#define USBDELAY            5          // Delay in MS for the main program loop - needed to not flood USB
#define LED7SEG_VCC         4



// NOTE: PINS 2 AND 3 ARE USED BY SDA/SCL AND CANNOT BE USED

/***********************************************************
*                         INCLUDES                         *
***********************************************************/
#include <Wire.h>                  //I2C Library
#include <Adafruit_LEDBackpack.h>  //Adafruit Backpack Libraries
#include <Adafruit_GFX.h>



//LIBRARY ATTACHMENTS
Adafruit_7segment LEDmatrix = Adafruit_7segment();


/***********************************************************
*                      GLOBAL VARS                         *
***********************************************************/
boolean  remoteBtnA_state = 0;       // Init variables and Set initial button states
boolean  remoteBtnD_state = 0;       // Init variables and Set initial button states
boolean  compState        = 0;       // Set computer status to default as off



JoyState_t joySt;


/***********************************************************
*                          SETUP                           *
***********************************************************/
void setup(){
        // Assign pin modes
        pinMode(XAXISPIN, INPUT); 
        pinMode(YAXISPIN, INPUT);   
        pinMode(RUDDPIN, INPUT);            
        pinMode(THROTPIN, INPUT);           
        
        pinMode(JOYPIN1, INPUT);             
        pinMode(JOYPIN2, INPUT);            
        pinMode(JOYPIN3, INPUT);             
        pinMode(JOYPIN4, INPUT);              
        pinMode(MODEPINAUTO, INPUT);        
        pinMode(MODEPINMAN, INPUT);         
        pinMode(SIMRESETPIN, INPUT);          
        pinMode(REMOTEIN_ON, INPUT);                
        pinMode(REMOTEIN_OFF, INPUT);         
        pinMode(COMPUTER_PWR, OUTPUT);              
        pinMode(COMPUTER_SENSE, INPUT);             
        



	joySt.xAxis    = 0;
	joySt.yAxis    = 0;
	joySt.throttle = 0;
        joySt.rudder   = 0;
	joySt.buttons  = 0;

LEDmatrix.begin(0x70);

}


/***********************************************************
*                          LOOP                            *
***********************************************************/
void loop(){
/*      // OLD STUFF
	joySt.xAxis = random(1023);
	joySt.yAxis = random(1023);
	//joySt.zAxis = random(1023);
	//joySt.throttle = random(1023);
	

//        joySt.throttle++;
//	joySt.rudder++;

	joySt.buttons <<= 1;
	if (joySt.buttons == 0)
		joySt.buttons = 1;
*/
  readModeState;
  
  readRemoteButtons;
  
  readAnalogControls;

	delay(USBDELAY);

	Joystick.setState(&joySt);    // Send that data bits 

}

/***********************************************************
*                 readAnalogControls                       *
***********************************************************/
void readAnalogControls(){
  
  // Reads the analog input at full 10bit, and sets joystick variables (16bit) directly
  joySt.xAxis = analogRead(XAXISPIN);
  joySt.yAxis = analogRead(YAXISPIN);
  joySt.rudder = analogRead(RUDDPIN);
  joySt.throttle = analogRead(THROTPIN);

}



/***********************************************************
*                   readButtonStates                       *
***********************************************************/
void readButtonStates (){
  // read the pins and record states
  // bitshift into joySt.button
}


/***********************************************************
*                     readModeState                        *
***********************************************************/
void readModeState (){
  // read and set flags for loop
  // possibly integrate into main loop instead
}


/***********************************************************
*                   readRemoteButtons                      *
***********************************************************/
void readRemoteButtons (){
  // read the button /remote controls
  // Read both digitial inputs for the RF remote buttons and store as state
  remoteBtnA_state = digitalRead(REMOTEIN_ON);
  remoteBtnD_state = digitalRead(REMOTEIN_OFF);
  // looking at using pins 12/13 for the input. so. many. pins.  
}

/***********************************************************
*               readComputerPowerStates                    *
***********************************************************/
void readComputerPowerStates(){
    // Read the computer LED power on pin and store as computer state
    compState = digitalRead(COMPUTER_SENSE);
}

/***********************************************************
*                     turnComputerOn                       *
***********************************************************/
void turnComputerOn (){
  
readComputerPowerStates();
  
//  Serial.print("turn on computer - state = ");
 // Serial.println(compState);
  
  if (!compState) {                     // If the computer is NOT on
    pinMode (COMPUTER_PWR, OUTPUT);     // Set control pin to output
    digitalWrite(COMPUTER_PWR, LOW);    // send the pin LOW (press btn to gnd) for 100 ms
  }
 
 delay(100);
 
 if (!compState) {                      // Sets the computer control pin to high-impedance
   pinMode(COMPUTER_PWR, INPUT);        // so that the on-board button still works
 }
 
// Serial.println("computer should be on now");
}


/***********************************************************
*                     turnComputerOff                      *
***********************************************************/
void turnComputerOff (){

  readComputerPowerStates();
 
//  Serial.print("turn off computer - state = ");
 // Serial.println(compState);
 
  if (compState) {                        // If the computer is ON
    pinMode (COMPUTER_PWR, OUTPUT);       // set the control pin mode to OUTPUT
    digitalWrite(COMPUTER_PWR, LOW);      // Send the pin LOW (press ptn to gnd) for 100ms
  }
  
 delay(100);                    
 
 if (compState) {                         // Sets the computer control pin to high-impedance 
   pinMode(COMPUTER_PWR, INPUT);          // so that the on-board button still works
 }

//  Serial.println("computer should be off now");
}


/***********************************************************
*                        LEDDisplay                        *
***********************************************************/
void LEDDisplay(){
 // code to write out to the display here 
}
