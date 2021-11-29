#ifndef NET_ELUM_BUTTON
#define NET_ELUM_BUTTON

#include <Arduino.h>

namespace Net_Elum {
  class Button {
    public:
      //ctor
      Button(uint8_t eventCount);

      bool IsUp();
      bool IsDown();
      bool IsClicked();
      
      //Call Update State once per loop to update the internal state and counters
      void UpdateState(bool buttonPressed);

    private:
      uint8_t _state;              //1=Up, 2=Down, 4=Clicked, 8=Autoreleased
      uint8_t _counter;            //Count pulses of pin high, capped at _eventCount
      uint8_t _eventCount;         //How many UpdateState calls will cause a button state change
  };
}
#endif
