#include <matrixScanner.h>
/*  Matrix Scanning Logic

All columns are input with pullup

All Rows are high when not in use 
=> high on row and col means nothing happening

For each row:
  Set the row to low, then check each column
  If it's low, buttton pressed

so Row's are out, Cols are in
*/

void MatrixScanner::Begin() {
  
  //Setup Columns

  DDRB &= B10011111;    // B5 & B6
  DDRD &= B01101111;    // D7 & D4
  PORTB |= B01100000;   // B5 & B6 pullup
  PORTD |= B10010000;   // D7 & D4 pullup

  //Setup Rows
  DDRC |= B01000000;    // C6
  DDRE |= B01000000;    // E6
  DDRF |= B00110000;    // F5 & F4

  PORTC |= B01000000;   //Set rows high
  PORTE |= B01000000;
  PORTF |= B00110000;

  rowNum = 0;   //Start at the first row
  MatrixState = 0;  //Default that all the buttons are up
  innerState = 0;
  IsValid = true;
}

//This scans one column, must be called repeatably
void MatrixScanner::UpdateMatrix() {
  IsValid = false;

  //Set the correct row to low
  if (rowNum == 0) {
    PORTC &= B10111111;
  } else if (rowNum == 1) {
    PORTE &= B10111111;
  } else if (rowNum == 2) {
    PORTF &= B11101111;
  } else {
    PORTF &= B11011111;
  }

  uint8_t rowShift = rowNum << 2;    //Multiply by 4
  uint16_t row = (PINB & B01100000) | (PIND & B10010000);
  uint16_t mask = B00001111;      //Mask for last four bits

  row = row >> 4;           //Shift into lower nibble
  row = ~row;               //Turn low state on into a 1
  row &= mask;              //Keep only the last nibble
  if (rowShift > 0) {
    row = row << rowShift;    //Shift to the correct place
    mask = mask << rowShift;  //Also shift the mask
  }

  innerState &= ~(mask);   //Turn off all the bits I'm thinking off
  innerState |= row;       //Turn them back on if needed

  rowNum++;
  if (rowNum == 4) {
    rowNum = 0;  
    MatrixState = innerState;
    IsValid = true;   
  }
    
  // Set the rows high
  PORTC |= B01000000;   //Set rows high
  PORTE |= B01000000;
  PORTF |= B00110000;
}