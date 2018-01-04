/*
Arduino Micro Joystick Control - Designed for OSC Exhibits

Required Libraries Adafruit_GFX Adafruit_LEDBackpack

Creates a joystick control for 4 analog and 16 button inputs using an arduino micro. Also has provisions for 7 Segment timer display and reset, along with RF Remote Button control for system on/off.

Created by Hybridsix (Michael King)
*/


/***********************************************************
*                         DEFINES                          *
***********************************************************/

#define SERIAL_DEBUG        0     // Set to 1 for helpful serial data output. Set to 0 if you want to just set up your own debugging

// Analog Input Defines
#define XAXISPIN           A0
#define YAXISPIN           A1
#define RUDDPIN            A2
#define THROTPIN           A3

// OLD Joystick Physical Button Pin Defines
#define JOYPIN1             6
#define JOYPIN2             8
#define JOYPIN3            10  
#define JOYPIN4            12      // Duplicaated address for REMOTE_ON - both will turn on the system if it is off. (Brakes?)

// General PIN and Button Defines
#define MODEPINAUTO         5
#define MODEPINMAN          7
#define SIMRESETPIN         9  
#define REMOTEIN_ON         12       // RF Remote input -  Pin D3(A) from RF - ON control ---- Duplicaated as define JOYPIN4 
#define REMOTEIN_OFF        13       // RF Remote input -  Pin D0(D) from RF - OFF control
#define COMPUTER_PWR        11       // Computer power button pin - ground for press
#define COMPUTER_SENSE      A5       // Computer power LED pin - tied to +5v of computer PWR LED pin

// General Defines
#define USBDELAY            10          // Delay in MS for the main program loop - needed to not flood USB
#define LED7SEG_VCC         4
#define SYSMODEOFF          0
#define SYSMODEAUT          1
#define SYSMODEMAN          2

// NOTE: PINS 2 AND 3 ARE USED BY SDA/SCL AND CANNOT BE USED

/***********************************************************
*                         INCLUDES                         *
***********************************************************/
#include <Wire.h>                  //I2C Library

/***********************************************************
*                      GLOBAL VARS                         *
***********************************************************/
boolean  remoteBtnA_state = 0;       // Init variables and Set initial button states
boolean  remoteBtnD_state = 0;       // Init variables and Set initial button states
boolean  compState        = 0;       // Set computer status to default as off
byte     modeStatus       = 1;       // byte to store mode values in (0-Off, 1-auto, 2-hand)  
uint32_t timeMillis       = 0;      
uint16_t btnDataPrev      = 0;       // a previous value to compare against to see if we need to write changes
uint16_t btnData          = 0;       // Setup for full 16 bit/buttons - only using 4 in existing setup. 

JoyState_t joySt;

/***********************************************************
*                          SETUP                           *
***********************************************************/
void setup(){
        // Assign pin modes
        
        //Analogs
        pinMode(XAXISPIN, INPUT); 
        pinMode(YAXISPIN, INPUT);   
        pinMode(RUDDPIN, INPUT);            
        pinMode(THROTPIN, INPUT);      
        
        //JoystickButtons
        pinMode(JOYPIN1, INPUT_PULLUP);             
        pinMode(JOYPIN2, INPUT_PULLUP);            
        pinMode(JOYPIN3, INPUT_PULLUP);             
        pinMode(JOYPIN4, INPUT_PULLUP);              
   
        // Other IO  
        pinMode(MODEPINAUTO, INPUT);        
        pinMode(MODEPINMAN, INPUT);         
        pinMode(SIMRESETPIN, INPUT_PULLUP);          
        // pinMode(REMOTEIN_ON, INPUT);                
        pinMode(REMOTEIN_OFF, INPUT);         
        pinMode(COMPUTER_PWR, OUTPUT);              
        pinMode(COMPUTER_SENSE, INPUT);             
        
        // Set/Reset all the default joystick values to 0
	joySt.xAxis    = 0;
	joySt.yAxis    = 0;
	joySt.throttle = 0;
        joySt.rudder   = 0;
	joySt.buttons  = 0;

Serial.begin(115200);

//LEDmatrix.begin(0x70);
}


/***********************************************************
*                          LOOP                            *
***********************************************************/
void loop(){
 // readModeState();
  switch (modeStatus) {
     case 0: {         // OFF
       if(compState){
       turnComputerOff();
       }
       break;
       // Do nothing?
     }
     case 1: {
        if (SERIAL_DEBUG == 1){Serial.println("auto");}
  //      readRemoteButtons();
        readButtonStates();
        readAnalogControls();
    //    LEDDisplay();
        break;
        //GameTimer;
        // And do the auto things, not because they are easy, but becasue they are hard
     }
     case 2: {
        //readRemoteButtons();
        readButtonStates();
        readAnalogControls();
       // LEDDisplay();
        break;
        //GameManualReset;
       // We choose to goto the man in this century
     }
     default:
     if (SERIAL_DEBUG == 1){Serial.println (" Case statement error");}
  }
	delay(USBDELAY);
	Joystick.setState(&joySt);    // Send that data bits 

}

/***********************************************************
*                 readAnalogControls                       *
***********************************************************/
void readAnalogControls(){
  if (SERIAL_DEBUG == 1){Serial.println("reading analog");}

  joySt.xAxis = map(analogRead(XAXISPIN), 0, 900, 0, 255);
  joySt.yAxis = map(analogRead(YAXISPIN), 930, 0, 0, 255);
  joySt.rudder = analogRead(RUDDPIN);
  joySt.throttle = map(analogRead(THROTPIN), 730, 290, 255, 0);
  
  Serial.println(analogRead(YAXISPIN));
}



/***********************************************************
*                   readButtonStates                       *
***********************************************************/
void readButtonStates(){
    // the code to make the button do 
  
  // Code below is sloppy but it works. Look for more elegant button solution later. 
  btnData += (digitalRead(SIMRESETPIN));
  btnData <<= 1;  
  btnData += (1);          // Button 14
  btnData <<= 1;
  btnData += (1);          // Button 13
  btnData <<= 1;
  btnData += (1);          // Button 12   
  btnData <<= 1;  
  btnData += (1);          // Button 11
  btnData <<= 1;
  btnData += (1);          // Button 10 
  btnData <<= 1;         
  btnData += (1);          // Button 9
  btnData <<= 1;
  btnData += (1);          // Button 8
  btnData <<= 1;
  btnData += (1);          // Button 7
  btnData <<= 1;
  btnData += (1);          // Button 6
  btnData <<= 1;
  btnData += (1);          // Button 5
  btnData <<= 1;      
  btnData += (1);          // Button 4
  // First 4
  btnData <<= 1;
  btnData += (digitalRead(JOYPIN4));  // Button 3
  btnData <<= 1;  
  btnData += (digitalRead(JOYPIN3));  // Button 2;
  btnData <<= 1;
  btnData += (digitalRead(JOYPIN2));  // Button 1;  
  btnData <<= 1;    
  btnData += (digitalRead(JOYPIN1));  // Button 0
 
//if (btnData != btnDataPrev)  {          // only write out to the variable if there is a change//
if (1){
      joySt.buttons = ~btnData;       // Inverts the Pulled up values to be low and writes to 
    if (SERIAL_DEBUG == 1){Serial.println("Data changed");}       // joySt.buttons variable
    if (SERIAL_DEBUG == 1){Serial.print("Joystick button: "); Serial.println(joySt.buttons, BIN);}
  btnDataPrev = btnData;
   }
btnData = 0; 
}


/***********************************************************
*                     readModeState                        *
***********************************************************/
void readModeState(){
  if (SERIAL_DEBUG == 1){Serial.println("reading mode pins");}
   if (digitalRead(MODEPINAUTO)){
     
     modeStatus = 1;   // auto
     }
   else if (digitalRead(MODEPINMAN)){
     modeStatus = 2;  // manual
     }
   else {
     modeStatus = 0;   // off
   }
  // read and set flags for loop
  // possibly integrate into main loop instead?
}


/***********************************************************
*                   readRemoteButtons                      *
***********************************************************/
void readRemoteButtons(){
  // read the button /remote controls
  // Read both digitial inputs for the RF remote buttons and store as state
  remoteBtnA_state = digitalRead(REMOTEIN_ON);
  remoteBtnD_state = digitalRead(REMOTEIN_OFF);
  // looking at using pins 12/13 for the input. so. many. pins.  
  
  //
  if (remoteBtnA_state){
    turnComputerOn;
  }
  if (remoteBtnD_state){
    turnComputerOff;
  }
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
void turnComputerOn(){
  
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
void turnComputerOff(){

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

  // Serial.println("computer should be off now");
}
