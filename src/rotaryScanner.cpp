#include <rotaryScanner.h>

/*

     Clockwise ->
     _______     _______
A ___|     |_____|     |____

        _______     _______
B ______|     |_____|     |____

     <- Counter Clockwise

https://www.best-microcontroller-projects.com/rotary-encoder.html

*/

void RotaryScanner::Begin() {
  //Set pins as inputs with pullups
  DDRB  &= B11100001;    // B1 - B4 input
  PORTB |= B00011110;   // B1 - B4 pullup

  DDRD  &= B11110000;    // D0 - D3 input
  PORTD |= B00001111;   // D0 - D3 pullup
}

void RotaryScanner::Update() {
  uint8_t dataPins = (PINB & B00011110) >> 1;   // PinA shifted to the bottom nibble
  uint8_t clockPins = (PIND & B00001111);        //PinB in bottom nibble

  for (uint8_t i=0; i<4; i++) {
    _deltas[i] += readRotary(i, (clockPins & B00000001), (dataPins & B00000001));

    dataPins  = dataPins >> 1;
    clockPins = clockPins >> 1;
  }
}

int8_t RotaryScanner::readRotary(int8_t id, bool clock, bool data) {
  static int8_t rot_enc_table[] = {0,1,1,0,1,0,0,1,1,0,0,1,0,1,1,0};

  _prevNextCode[id] <<= 2;
  if (data) {_prevNextCode[id] |= 0x02;}
  if (clock) {_prevNextCode[id] |= 0x01;}
  _prevNextCode[id] &= 0x0f;

  if (rot_enc_table[_prevNextCode[id]] ) {
    _store[id] = _store[id] << 4;
    _store[id] = _store[id] | _prevNextCode[id];
    if ((_store[id] & 0xff) == 0x2b) return -1;
    if ((_store[id] & 0xff) == 0x17) return 1;
  }
  return 0;
}

int8_t RotaryScanner::NextDelta(uint8_t id) {
  if (_deltas[id] > 0) {
    _deltas[id]--;
    return 1;
  } else if (_deltas[id] < 0) {
    _deltas[id]++;
    return -1;
  }
  return 0;
}