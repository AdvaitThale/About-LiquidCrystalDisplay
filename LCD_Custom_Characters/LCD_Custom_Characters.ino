/* 
 *  Author: Advait Thale

  *******************************************************************
  *                        LCD from Scratch                         *    
  *******************************************************************

  This is a very basic code to understand how a 8*1/8*2/16*2/20*4 & other LCDs works.
  Refer page 24 of HITACHI HD44780 datasheet for furthur details.
  
  The most common LCD pinout:
  ------------------------------------------------------------------------------
  |5V|GND|Contrast|Register Select|R/W|Enable|D0|D1|D2|D3|D4|D5|D6|D7|LED+|LED-| 
  ------------------------------------------------------------------------------
  ------------------------------------------------------------------------------
                            +---------+                                       |GND|
                            | PCF8574 |                                       |VCC|
                            | Module  |                                       |SDA|
                            +---------+                                       |SCL|
  ------------------------------------------------------------------------------
  This code is written with LiquidCrystal_I2C.h library which infact is used for PCF8574 I2C module.
*/

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

// byte heart[9] = {
//   0b00000,
//   0b01010,
//  0b11111,
//  0b11111,
//  0b01110,
//  0b00100,
//  0b00000,
//  0b00000
// };

//  byte battery[10] = {
//  0b01110,
//  0b10001,
//  0b10001,
//  0b10001,
//  0b10001,
//  0b10001,
//  0b11111,
//  0b11111
//  };


//   byte lock[8] = {
// 0b11111,
// 0b10001,
// 0b10001,
// 0b11111,
// 0b11011,
// 0b01111,
// 0b11111,
// 0b00000
// };

byte unlocked[14]{
    0b11111,
    0b10001,
    0b00001,
    0b11111,
    0b11111,
    0b11011,
    0b11011,
    0b11111
};

// byte hlines[15]{
//     0b11111,
//     0b00000,
//     0b11111,
//     0b00000,
//     0b11111,
//     0b00000,
//     0b11111,
//     0b00000
// };

// byte inv_hlines[16]{
//     0b00000,
//     0b11111,
//     0b00000,
//     0b11111,
//     0b00000,
//     0b11111,
//     0b00000,
//     0b11111
// };

// byte pitch[8] = 
//  {
//   0b00100,
//   0b01110,
//   0b10101,
//   0b00100,
//   0b00100,
//   0b00100,
//   0b00010,
//   0b00001
// };

 byte full_battery[10] = {
 0b01110,
 0b11111,
 0b11111,
 0b11111,
 0b11111,
 0b11111,
 0b11111,
 0b11111
 };

 byte ff_battery[12] = {
 0b01110,
 0b10001,
 0b10001,
 0b10001,
 0b11111,
 0b11111,
 0b11111,
 0b11111
 };

 byte tf_battery[13] = {
 0b01110,
 0b10001,
 0b10001,
 0b10001,
 0b10001,
 0b11111,
 0b11111,
 0b11111
 };

byte low_bat[11] = {
 0b01110,
 0b10001,
 0b10101,
 0b10101,
 0b10001,
 0b10101,
 0b10001,
 0b11111
 };

byte rays[17]{
  0b10000;
  0b01000;
  0b00100;
  0b00011;
  0b00011;
  0b00100;
  0b01000;
  0b10000;
};

void setup()
{
  Serial.begin(115200);
  lcd.init();
  lcd.backlight();
  lcd.clear();
// lcd.createChar(8, battery);
}

void loop ()
{
  lcd.setCursor(1, 0); //(0(Column),1(Row)) From 0>>1
  lcd.print("");

  lcd.createChar(10, full_battery);
  lcd.setCursor(1, 0);
  lcd.write(byte(10));

  lcd.createChar(12, ff_battery);
  lcd.setCursor(3, 0);
  lcd.write(byte(12));

  lcd.createChar(13, tf_battery);
  lcd.setCursor(5, 0);
  lcd.write(byte(13));
  
  lcd.createChar(11, low_bat);
  lcd.setCursor(7, 0);
  lcd.write(byte(11));

  lcd.createChar(14,unlocked);
  lcd.setCursor(9, 0);
  lcd.write(byte(14));
  
}
