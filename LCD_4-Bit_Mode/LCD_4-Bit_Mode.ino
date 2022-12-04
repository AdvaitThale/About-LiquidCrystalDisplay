/*
  Author: Advait Thale

  *******************************************************************
  *                        LCD 4-bit Mode                           *    
  *******************************************************************

  This is amore of advance and cost effective technique to implement on a 8*1/8*2/16*2/20*4 & other LCD.
  Here, only 4 pin are used for data instead of 8 and 2 are used for RS and EN to control the LCD.
  Refer page 24 of HITACHI HD44780 datasheet for furthur details.
  
  The most common LCD pinout:
  ------------------------------------------------------------------------------
  |5V|GND|Contrast|Register Select|R/W|Enable|D0|D1|D2|D3|D4|D5|D6|D7|LED+|LED-| 
  ------------------------------------------------------------------------------
*/


#include<LiquidCrystal.h>

LiquidCrystal lcd(12, 11, 10, 9, 8, 7); // (RS, En, D4, D5, D6, D7)

void setup()
{
  Serial.begin(115200);
  lcd.begin(16, 2);
  lcd.clear();
}
void loop()
{
  lcd.setCursor(0, 0); //(Column, Row)
  lcd.print("Hello, Earth!!");
  lcd.clear();
}
