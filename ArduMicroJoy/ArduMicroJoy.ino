
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
#define USBDELAY            5          // Delay in MS for the main program loop - needed to not flood USB
#define LED7SEG_VCC         4
#define SYSMODEOFF          0
#define SYSMODEAUT          1
#define SYSMODEMAN          2

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
byte     modeStatus       = 1;       // byte to store mode values in (0-Off, 1-auto, 2-hand)  
uint32_t timeMillis       = 0;      
boolean btnArrayPrev[16];                    // a previous value to compare against to see if we need to write changes


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
        pinMode(JOYPIN1, INPUT);             
        pinMode(JOYPIN2, INPUT);            
        pinMode(JOYPIN3, INPUT);             
        pinMode(JOYPIN4, INPUT);              
   
        // Other IO  
        pinMode(MODEPINAUTO, INPUT);        
        pinMode(MODEPINMAN, INPUT);         
        pinMode(SIMRESETPIN, INPUT);          
        pinMode(REMOTEIN_ON, INPUT);                
        pinMode(REMOTEIN_OFF, INPUT);         
        pinMode(COMPUTER_PWR, OUTPUT);              
        pinMode(COMPUTER_SENSE, INPUT);             
        
      // Turn on pullups for joystick buttons
        digitalWrite(JOYPIN1, HIGH);
        digitalWrite(JOYPIN2, HIGH);
        digitalWrite(JOYPIN3, HIGH);
        digitalWrite(JOYPIN4, HIGH);

      // Set/Reset all the default joystick values to 0
	joySt.xAxis    = 0;
	joySt.yAxis    = 0;
	joySt.throttle = 0;
        joySt.rudder   = 0;
	joySt.buttons  = 0;

Serial.begin(9600);

//LEDmatrix.begin(0x70);
}


/***********************************************************
*                          LOOP                            *
***********************************************************/
void loop(){
  Serial.println("loop");
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
        Serial.println("auto");
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
     Serial.println (" Case statement error");  
  }
	delay(USBDELAY);
	Joystick.setState(&joySt);    // Send that data bits 

}

/***********************************************************
*                 readAnalogControls                       *
***********************************************************/
void readAnalogControls(){
  Serial.println("reading analog");
  // Reads the analog input at full 10bit, and sets joystick variables (16bit) directly
  joySt.xAxis = analogRead(XAXISPIN);
  joySt.yAxis = analogRead(YAXISPIN);
  joySt.rudder = analogRead(RUDDPIN);
  joySt.throttle = analogRead(THROTPIN);

}



/***********************************************************
*                   readButtonStates                       *
***********************************************************/
void readButtonStates(){
    // the code to make the button do 
      Serial.println("reading Buttons");
 boolean btnArray[16];                        // Setup for full 16 bit/buttons - only using 4 in existing setup. 
 boolean btnChange = 0;
  
  btnArray[0]  = digitalRead(JOYPIN1);
  btnArray[1]  = digitalRead(JOYPIN2);
  btnArray[2]  = digitalRead(JOYPIN3);
  btnArray[3]  = digitalRead(JOYPIN4);
 // btnArray[15] = digitalRead(SIMRESETPIN); 
  

   for (int i = 0; i < 16; i++){              // checks to see if there is a change between 
     if (btnArray[i] != btnArrayPrev[i]){     // rounds of button reading
       btnChange = 1;
       Serial.println("button changed!");
       break;
     }
     else {
       btnChange = 0;
     }     
   } 

  if (btnChange){             // If its changed, then write out the var
    for (byte i = 0; i < 16; i++){   
      Serial.print(btnArray[i], DEC);      
      joySt.buttons <<= i;
      joySt.buttons += btnArray[i];
        for (byte i = 0; i < 16; i++){          // copy read states to btnArrayPrev[]
          btnArrayPrev[i] = btnArray[i];
        }
      Serial.println();  
  }

 //    if (btnArray[15]) {                        // if the reset button is pressed
 //   delay(10);        // Some delays to soften the blow and debounce
  //  resetFlight = 0; // Set the button state back to 0, don't care what the switch says
 //   delay(1000);
 // }
  // read the pins and record states
  // bitshift into joySt.button
  }
}


/***********************************************************
*                     readModeState                        *
***********************************************************/
void readModeState(){
  Serial.println("reading mode pins");
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

//  Serial.println("computer should be off now");
}


/***********************************************************
*                        LEDDisplay                        *
***********************************************************/
void LEDDisplay(){
 // code to write out to the display here 
}



























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

