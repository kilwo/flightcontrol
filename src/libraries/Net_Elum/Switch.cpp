#include "Switch.h"

namespace Net_Elum {

  #define StateOffReporting   B1
  #define StateOffIdle       B10
  #define StateOnReporting  B100
  #define StateOnIdle      B1000

  //A switch will indicate a change of state for one update when it is moved from off to on or viceversa

  Switch::Switch(uint8_t newEventCount, uint16_t newSendCount) {
    state = StateOffIdle;
    counter = 0;
    eventCount = newEventCount;
    sendCount = newSendCount;
    sendCounter = 0;
  }

  //Report nothing if the switch is idle
  bool Switch::IsOn() {
    return (state == StateOnReporting);
  }

  bool Switch::IsOff() {
    return (state == StateOffReporting);
  }

  //1=Off-Reporting  2=Off-Idle  4=On-Reporting  8=On-Idle
  void Switch::UpdateState(bool switchOn) {
    if (sendCounter == 0) {
      //Stop reporting
      if (state & (StateOffReporting | StateOnReporting)) {
        state = state << 1;   //Idles are always double reporting
      }
    } else {
      //Keep reporting and decrement the counter
      sendCounter--;
    }
    
    if (switchOn) {
      if (counter != eventCount) {
        counter++;
      }

      if (counter == eventCount) {
        //If it is in some state of off-ness
        if (state & (StateOffReporting | StateOffIdle)) {
          state = StateOnReporting;         //Report on
          sendCounter = sendCount;          //Start counter
        }
      }

    } else {
      if (counter != 0) {
        counter--;
      }

      if (counter == 0) {
        //If it is in some state of on-ness
        if (state & (StateOnReporting | StateOnIdle)) {
          state = StateOffReporting;        //Report off
          sendCounter = sendCount;          //Start counter
        }
      }
    }
  }
}