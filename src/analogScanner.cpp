#include <analogScanner.h>
/*  Analog Scanning Logic
*/

#define ADCA3 B0100
#define ADCA2 B0101
#define ADCA1 B0110
#define ADCA0 B0111

#define ADCWorking (ADCSRA & B01000000)

void AnalogScanner::Begin() {
  _A0Val = 0;
  _A1Val = 0;

  A0Val = 0;
  A1Val = 0;
  
  HasChanged = false;
  _modeA0 = true;

  DDRF &= B00111111;    // F7 & F6 as inputs
  PORTF &= B00111111;   // F7 & F6 no pullup

  ADCSRA = B10000110;   // turn ADC on with prescaler if 128
  ADMUX = B01000000;  // Voltage Reference Selection - AVcc with external capacitor on AREF pin = Default
}

void AnalogScanner::Update() {
  HasChanged = false;

  if (_modeA0) {
    if (!ADCWorking) {
      _A0Val = map(ADC, 0, 1024, 0, 101);
      _modeA0 = false;
      Start(ADCA1);
    }
  } else {
    if (!ADCWorking) {
      _A1Val = map(ADC, 0, 1024, 0, 101); 
      _modeA0 = true;
      Start(ADCA0);
      
      if ((_A0Val != A0Val) || (_A1Val != A1Val)) {
        A0Val = _A0Val;
        A1Val = _A1Val;
        HasChanged = true;
      }
    }
  }
}

void AnalogScanner::Start(uint8_t adc_port) {
  if (!ADCWorking) {
    ADMUX = (ADMUX & B11111000) | (B00000111 & adc_port);  //Set the port
    ADCSRA |= B01000000;              //Set ADSC to start conversion  
  }
}
