#include "Button.h"

namespace Net_Elum {

  //1=Up, 2=Down, 4=Clicked, 8=Autoreleased, 16=Auto-Clickec

  #define StateUp              B1
  #define StateDown           B10
  #define StateClicked       B100

  //A switch will indicate a change of state for one update when it is moved from off to on or viceversa

  Button::Button(uint8_t eventCount) {
    _state = StateUp;
    _counter = 0;
    _eventCount = eventCount;
  }

  //Report nothing if the switch is idle
  bool Button::IsUp() {
    return (_state & (StateUp | StateClicked));
  }

  bool Button::IsDown() {
    return (_state & StateDown);
  }

  bool Button::IsClicked() {
    return (_state & StateClicked);
  }

  void Button::UpdateState(bool buttonPressed) {
    //Stop reporting click
    if (_state == StateClicked) {
      _state = StateUp;
    } 
          
    if (buttonPressed) {
      if (_counter == _eventCount) {
        //Button has been pressed long enough to count
        _state = StateDown;
      } else {
        _counter++;
      }
      
    } else {
      if (_counter == 0) {
        if (_state & (StateDown)) {
          _state = StateClicked;
        }
      } else {
        _counter--;
      }
    }
  }
}