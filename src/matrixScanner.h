#ifndef MATRIX_SCANNER
#define MATRIX_SCANNER

#include <Arduino.h>

class MatrixScanner {
	public:
		void Begin();
    void UpdateMatrix();

    bool IsValid;
    uint16_t MatrixState;

	private:
    uint8_t rowNum;
    uint16_t innerState;

};

#endif