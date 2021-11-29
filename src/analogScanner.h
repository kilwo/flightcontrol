#ifndef ANALOG_SCANNER
#define ANALOG_SCANNER

#include <Arduino.h>

class AnalogScanner {
	public:
		void Begin();
    void Update();

    int A0Val;
    int A1Val;

    bool HasChanged;

  private:
    void Start(uint8_t adc_port);

    int _A0Val;
    int _A1Val;

    bool _modeA0;

};

#endif