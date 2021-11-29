#ifndef ROTARY_SCANNER
#define ROTARY_SCANNER

#include <Arduino.h>

class RotaryScanner {
	public:
		void Begin();
    void Update();

    int8_t NextDelta(uint8_t id);
    
  private:
    int8_t readRotary(int8_t id, bool data, bool clock);

    uint8_t _prevNextCode[4] = {0,0,0,0};
    uint16_t _store[4] = {0,0,0,0};
    int8_t _deltas[4] = {0,0,0,0};              //Deltas are the changes that need to be sent for each encoder
};

#endif