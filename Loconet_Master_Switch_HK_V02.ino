/************************************************************************************************************
 *
 *  Copyright (C) 2015-2016 Timo Sariwating
 *  Edit by Septillion (Timo Engelgeer) January 2, 2016
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with this program; if not, If not, see <http://www.gnu.org/licenses/>.
 *
 ************************************************************************************************************

DESCRIPTION:
This is a Loconet OPC_GPON and OPC_GPOFF Switch
- It uses an Arduino Pro Mini 5v 16MHz or a Arduino Nano ATMega328P/ATMega168

Loconet:
- TX pin D7
- RX pin D8
You MUST connect the RX input to the AVR ICP pin which on an Arduino UNO is digital pin 8.
The TX output can be any Arduino pin, but the LocoNet library defaults to digital pin 6 for TX

Libraries:
Uses the Bounce2 an Loconet libraries. They need to be installed in order to compile.
Bounce2: https://github.com/thomasfredericks/Bounce2
Loconet: https://github.com/mrrwa/LocoNet

Edits Monobrachius 2022
-added latching E-stop option that will keep E-stop when pushed (in case of receiving an external OPC_GPON, this module will send a OPC_GPOFF since the Emergency Stop button is still pushed and power on track is not allowed).
-added the possibility to have NO E-stop button for people that do not want to keep to standard safety practices ;-)
-added blinking to red led in case it is pushed (so user can see which E-stop button is pushed/it failed)
-added the possibility to have one button that switches between both signals (if it is green and it is pushed, it will send OPC_GPOFF, if it is red, it will send OPC_GPON), to totally ignore standard safety practices.
 Advantage: one button for all. Disadvantage, if you press twice, all trains will start to run again!
-added 

/************************************************************************************************************/
#include <LocoNet.h>
#include <Bounce2.h>

//Definitions
const int  EStopLatching              = 0;    //1 = latching (button does not fall back, but needs turning/pushing to return to "normal" position), 0 = momentary
const int  EStopNCtype                = 0;    //1 = NC, 0 = NO
const int  EStopOneButtonForAll       = 0;    //1 = one momentary button for set and reset, 0 for two buttons or a latching button
const int  EStopKeptActiveWhilePushed = 0;    //1 = Keeping E-Stop while pushed, 0 = E-Stop can be reset while button is still pushed.

//Buttons
const byte RedButtonPin   = 3;          //as defined in EStopNCtype   //PCB V2.0 pin 3, PCB V1.0 pin 11
const byte GreenButtonPin = 2;          //of NO type                  //PCB V2.0 pin 2, PCB V1.0 pin 12
//LEDs
const byte RedLed   = 9;                //PCB V2.0 pin 9, PCB V1.0 pin 9
const byte GreenLed = 5;                //PCB V2.0 pin 5, PCB V1.0 pin 10
const int ledPin =  LED_BUILTIN;// the number of the LED pin

// Loconet
#define LOCONET_TX_PIN 7
static lnMsg *LnPacket;
static LnBuf  LnTxBuffer;

// Blockwave generator
const long    interval        = 666;    // interval at which to blink (milliseconds)

// Variables will change:
boolean       OPCSTATE        = 0;      // For state control center (on/off)
int           blockwaveOutput = 0;      // Needed for the Blockwave generator
unsigned long previousMillis  = 0;      // Will store last the Blockwave generator was updated

Bounce buttonRed, buttonGreen; //The button objects of Bounce2 library


/*************************************************************************/
/*          Setup                                                        */
/*************************************************************************/ 
void setup() {
  
  // Setup the buttons
  buttonRed.attach(RedButtonPin, INPUT_PULLUP);
  buttonGreen.attach(GreenButtonPin, INPUT_PULLUP);
  if(EStopOneButtonForAll){
    pinMode(GreenButtonPin, OUTPUT);
    digitalWrite(GreenButtonPin,LOW);
  }
  
  
  // Set up the outputs
  pinMode(RedLed, OUTPUT);
  pinMode(GreenLed, OUTPUT);
  
  // Initialize the LocoNet interface
  LocoNet.init(LOCONET_TX_PIN); // The TX output in LocoNet library defaults to digital pin 6 for TX
  Serial.begin(9600);
  Serial.println("LocoNet Master Switch HK V0.1");
}

/*************************************************************************/
/*          Send OPC_GP                                                  */
/*************************************************************************/ 
void sendOPC_GP(byte on) {
  lnMsg SendPacket;
  if (on) {
      SendPacket.data[ 0 ] = OPC_GPON;  
  } else {
      SendPacket.data[ 0 ] = OPC_GPOFF;  
  }
  LocoNet.send( &SendPacket ) ;
}

/*************************************************************************/
/*          Program Loop                                                 */
/*************************************************************************/ 
void loop() {
  // Read the Buttons
  readButtons();
  
  // Check LocoNet for a power state update
  checkLocoNet();

  // Generate Blockwave
  Blockwave();
  
  // Set the LEDs
  setLed();  
}

/*************************************************************************/
/*          Read the Red and Green Buttons                               */
/*************************************************************************/ 
void readButtons() {
//Read the actual buttons
  buttonRed.update();
  buttonGreen.update();

//check for press of the red button (rose, because of the NC type or fell in case of NO)
  if(buttonRed.rose()*EStopNCtype or buttonRed.fell()*!EStopNCtype){
    if(!EStopOneButtonForAll){                              //so two buttons, one for set, one for reset, or for 1 latching button
      sendOPC_GP(0);  //Send new state to controle center
      OPCSTATE = 0;   //and save the new state
      Serial.println("Outgoing: Track Power off");
    }
    else{                                                   //one identical momentary button for set and reset
      if(OPCSTATE){                                         //track is powered, but power must be cut
        sendOPC_GP(0);  //Send new state to controle center
        OPCSTATE = 0;   //and save the new state
        Serial.println("Outgoing: Track Power off");
      }
      else{                                                 //track is off, but reset is required
        sendOPC_GP(1);  //Send new state to controle center
        OPCSTATE = 1;   //and save the new state
        Serial.println("Outgoing: Track Power on");
      }
    }
  }
  
//Check is the green button became pressed (fell, because of NO type) / red button became released (fell, because of NC type)
  else if(buttonGreen.fell()){
    if(!(EStopNCtype*digitalRead(RedButtonPin) or !EStopNCtype*!digitalRead(RedButtonPin))){ //E-Stop locally not pressed
      sendOPC_GP(1);  //Send new state to controle center
      OPCSTATE = 1;   //and save the new state
      Serial.println("Outgoing: Track Power on");
      }
    else{
      Serial.println("E-Stop locally still active, do not reset");
    }
  }
  else if(EStopLatching* (buttonRed.fell()*EStopNCtype or buttonRed.rose()*!EStopNCtype) ){
    sendOPC_GP(1);  //Send new state to controle center
    OPCSTATE = 1;   //and save the new state
    Serial.println("Outgoing: Track Power on");
  }
}

/*************************************************************************/
/*          Read the Red and Green Buttons                               */
/*************************************************************************/ 
void checkLocoNet() {
  // Check for any received LocoNet packets
  LnPacket = LocoNet.receive() ;
  if( LnPacket )
  {
    if (LnPacket->sz.command == OPC_GPON)  {        // GLOBAL power ON request 0x83    
      if(!EStopLatching or EStopNCtype*!digitalRead(RedButtonPin) or !EStopNCtype*digitalRead(RedButtonPin)){        //E-Stop externally reset/not locally active anymore 
        OPCSTATE = 1;
        Serial.println("Incoming: Track Power on");
      }
       else if(EStopKeptActiveWhilePushed && (EStopNCtype*digitalRead(RedButtonPin) or !EStopNCtype*!digitalRead(RedButtonPin))){ //E-Stop still here active, set track-power to off and send signal back
        OPCSTATE = 0;
        Serial.println("Incoming: Track Power on, but locally still active, resend E-stop again to network");
        sendOPC_GP(0);  //Send new state to controle center
       }
    } 
    else if (LnPacket->sz.command == OPC_GPOFF) { // GLOBAL power OFF req 0x82
      OPCSTATE = 0;
      Serial.println("Incoming: Track Power off");
    }
  }
}


/*************************************************************************/
/*          Set the Red and Green Leds                                  */
/*************************************************************************/ 
void setLed() {
  digitalWrite(RedLed, !OPCSTATE-blockwaveOutput*(EStopNCtype*digitalRead(RedButtonPin) or !EStopNCtype*!digitalRead(RedButtonPin)));
  digitalWrite(GreenLed, OPCSTATE);
}

/*************************************************************************/
/*          Generate Block Wave                                          */
/*************************************************************************/ 
void Blockwave() {

  // check to see if it's time to blink the LED; that is, if the difference
  // between the current time and last time you blinked the LED is bigger than
  // the interval at which you want to blink the LED.
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    // save the last time you blinked the LED
    previousMillis = currentMillis;

    // if the LED is off turn it on and vice-versa:
    if (blockwaveOutput == LOW) {
      blockwaveOutput = HIGH;
    } else {
      blockwaveOutput = LOW;
    }
    // set the LED with the ledState of the variable:
    //digitalWrite(ledPin, blockwaveOutput);
  }
}
