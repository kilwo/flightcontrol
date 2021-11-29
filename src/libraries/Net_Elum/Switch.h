#ifndef NET_ELUM_SWITCH
#define NET_ELUM_SWITCH

#include <Arduino.h>

namespace Net_Elum {
  class Switch {
    public:
      //ctor
      Switch(uint8_t newEventCount, uint16_t newSendCount);

      bool IsOn();
      bool IsOff();

      //Call Update State once per loop to update the internal state and counters
      void UpdateState(bool buttonPressed);

      uint16_t sendCounter;       //Count of events state has been reporting
      
    private:
      uint8_t state;               // 1=Off-Reporting  2=Off-Idle  4=On-Reporting  8=On-Idle
      uint8_t counter;             //Count pulses of pin high, capped at _eventCount
      uint8_t eventCount;		      //How many UpdateState calls will cause a button state change
      uint16_t sendCount;         //How long, in events, will this toggle report on or off

      
  };
}
#endif
