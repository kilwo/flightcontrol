#include <Arduino.h>
#include "libraries/Net_Elum/Button.h"
#include "libraries/Net_Elum/Switch.h"
#include "matrixScanner.h"
#include "analogScanner.h"
#include "rotaryScanner.h"
#include <Joystick.h>

MatrixScanner matrixScanner;
AnalogScanner analogScanner;
RotaryScanner rotaryScanner;

const uint8_t buttonEventCount = 100;
const uint16_t toggleSendTime = 200;
const uint16_t rotarySendTime = 200;

Joystick_ Joystick(
  JOYSTICK_DEFAULT_REPORT_ID, //hidReportId
  JOYSTICK_TYPE_JOYSTICK,     //joystickType
  26,                         //buttonCount
  0,                          //hatSwitchCount
  false,                      //includeXAxis
  true,                       //includeYAxis
  false,                      //includeZAxis
  false,                      //includeRxAxis
  true,                       //includeRyAxis
  false,                      //includeRzAxis
  false,                      //includeRudder
  false,                      //includeThrottle
  false,                      //includeAccelerator
  false,                      //includeBrake
  false                       //includeSteering
);

//A button handler for each of the 16 matrixed buttons and switches
Net_Elum::Button buttons[16] = {
    Net_Elum::Button(buttonEventCount),
    Net_Elum::Button(buttonEventCount),
    Net_Elum::Button(buttonEventCount),
    Net_Elum::Button(buttonEventCount),
    Net_Elum::Button(buttonEventCount),
    Net_Elum::Button(buttonEventCount),
    Net_Elum::Button(buttonEventCount),
    Net_Elum::Button(buttonEventCount),
    Net_Elum::Button(buttonEventCount),
    Net_Elum::Button(buttonEventCount),
    Net_Elum::Button(buttonEventCount),
    Net_Elum::Button(buttonEventCount),
    Net_Elum::Button(buttonEventCount),
    Net_Elum::Button(buttonEventCount),
    Net_Elum::Button(buttonEventCount),
    Net_Elum::Button(buttonEventCount)
};

/* Button mappings
[0..7] - Rotary Encoders
[8..11] - Rotary Buttons
[12..23] - Buttons and Switches
 - 15 & 19 are toggle inputs maping 15 -> [15, 24]   19 -> [19, 25]
*/

//The two toggles need to get the button state mapped over
// Switch event counts are set to 1 as the debounce happens in the button that handles the switch input
Net_Elum::Switch toggles[2] = {
  Net_Elum::Switch(1, toggleSendTime),
  Net_Elum::Switch(1, toggleSendTime)
};

//Use this to track if a button state change needs to be sent
bool buttonState[26] = {
  false, false, false, false, false, false, false, false, 
  false, false, false, false, false, false, false, false, 
  false, false, false, false, false, false, false, false, 
  false, false
};

//This is the last state of the buttons, diff used to check if anything needs to be sent
bool lastButtonState[26] = {
  false, false, false, false, false, false, false, false, 
  false, false, false, false, false, false, false, false, 
  false, false, false, false, false, false, false, false, 
  false, false
};

//Time out to turn a single event into a longer button press
uint16_t rotarySendTimers[4] = {0,0,0,0};

void setup() {
  
  /*while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB
  }*/
  

  //Call begin on each scanner to init the pinState
  matrixScanner.Begin();
  analogScanner.Begin();
  rotaryScanner.Begin();

  //Set the analog range to 0 to 100. 
  //Physical values will be scaled.
  Joystick.setYAxisRange(0,100);
  Joystick.setRyAxisRange(0,100);

  //Start the joystick
  Joystick.begin(false);
} 

// Superloop - No delays!
void loop() {
  
  //Do we need to send an update this pass through the loop()
  bool joystickNeedsUpdate = false;
  
  //copy the button state from this to last
  memcpy(lastButtonState, buttonState, sizeof(buttonState[0])*26);  
  
  //button index will be used to map physical inputs into the buttonState array
  uint8_t buttonIndex;

  // ***** Rotary Encoders ******
  rotaryScanner.Update();
  for (uint8_t i=0; i<4; i++) {
    buttonIndex = i << 1;     //Multiply by two
    uint8_t upIndex = buttonIndex++;

    if (rotarySendTimers[i] == 0) {
      //Check if there is any action to send
      int8_t delta = rotaryScanner.NextDelta(i);
    
      if (delta != 0) {
        if (delta > 0) {
          buttonState[upIndex] = true;        //Up clicks on the odd bit//
        } else {
          buttonState[buttonIndex] = true;
        }
        rotarySendTimers[i] = rotarySendTime;
      }
    } else {
      //Sending a action
      rotarySendTimers[i]--;

      if (rotarySendTimers[i] == 0) {
        //Set the state to off
        buttonState[buttonIndex] = false;
        buttonState[upIndex] = false;
      }
    }
  }  
  // ***** End Rotary Encoders ******

  // ***** Button Matrix ******
  matrixScanner.UpdateMatrix();                         //Update internal state to match physical state
  
  if (matrixScanner.IsValid) {                          //Matrix only valid once every 4 loops after all cols and rows scanned
    uint16_t localstate = matrixScanner.MatrixState;

    //Map matrix onto button array
    for (uint8_t i=0; i<16; i++) {
      buttons[i].UpdateState(localstate & B00000001);
      localstate = localstate >> 1;

      //Four rotary encoders up/down rotation are button 0-7, map above that
      buttonIndex = i+8;

      if (buttonIndex == 15) {
        //Button index 15 maps to switch buttons 15 and 24
        //15 is the off event, 24 is the on event
        toggles[0].UpdateState(buttons[i].IsDown());
        
        buttonState[15] = false;
        buttonState[24] = false;

        if (toggles[0].IsOff()) {
          buttonState[15] = true;
        }
        if (toggles[0].IsOn()) {
          buttonState[24] = true;
        }

      } else if (buttonIndex == 19) {
        //Button index 19 maps to switch buttons 19 and 25
        //19 is the off event, 25 is the on event
        toggles[1].UpdateState(buttons[i].IsDown());

        buttonState[19] = false;
        buttonState[25] = false;

        if (toggles[1].IsOff()) {
          buttonState[19] = true;
        }
        if (toggles[1].IsOn()) {
          buttonState[25] = true;
        }
        
      } else {
        if (buttons[i].IsDown()) {
          buttonState[buttonIndex] = true;
        } else {
          buttonState[buttonIndex] = false;
        }
      }
    }
  }

  for (uint8_t i=0; i<26; i++) {
    if (buttonState[i] != lastButtonState[i]) {
      //It has changed, we need to send it
      Joystick.setButton(i, buttonState[i]);
      joystickNeedsUpdate = true;
    }
  }
  // ***** End Button Matrix ******

  // ***** Analog Sliders ******
  analogScanner.Update();
  if (analogScanner.HasChanged) {
    Joystick.setYAxis(analogScanner.A0Val);
    Joystick.setRyAxis(analogScanner.A1Val);
    joystickNeedsUpdate = true;
  }
  // ***** End Analog Sliders ******

  // ***** Joystick sender ******
  // Only call sendState() if there has actually been a change in the physical / reported state of the controller
  if (joystickNeedsUpdate) {
    Joystick.sendState();
  }
}