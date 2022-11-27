/*
  Author: Advait Thale

  *******************************************************************
  *                        LCD from Scratch                         *    
  *******************************************************************

  This is a very basic code to understand how a 8*1/8*2/16*2/20*4 & other LCDs works.
  Refer page 24 of HITACHI HD44780 datasheet for furthur details.
  
  The most common LCD pinout:
  ------------------------------------------------------------------------------
  |5V|GND|Contrast|Register Select|R/W|Enable|D0|D1|D2|D3|D4|D5|D6|D7|LED+|LED-| 
  ------------------------------------------------------------------------------
*/


#define LCD_RS  10  // Register Select/RS -> 10
#define LCD_EN  11  // Enable/EN -> 11
/* Since, we are writing(displaying) data on LCD, Connect R/W to GND */

/* Total 8 Data Bits */
#define D0  2       // Data Pin 0 -> 2       
#define D1  3       // Data Pin 1 -> 3
#define D2  4       // Data Pin 2 -> 4
#define D3  5       // Data Pin 3 -> 5
#define D4  6       // Data Pin 4 -> 6
#define D5  7       // Data Pin 5 -> 7
#define D6  8       // Data Pin 6 -> 8
#define D7  9       // Data Pin 7 -> 9


void setup() {
  pinMode(LCD_EN, OUTPUT); 
  pinMode(LCD_RS, OUTPUT);
  pinMode(D0, OUTPUT);
  pinMode(D1, OUTPUT);
  pinMode(D2, OUTPUT);
  pinMode(D3, OUTPUT);
  pinMode(D4, OUTPUT);
  pinMode(D5, OUTPUT);
  pinMode(D6, OUTPUT);
  pinMode(D7, OUTPUT);
  Serial.begin(115200);
  Serial.println("\t  Connected:");

 /* For setting the LCD, pull RS pin to LOW and for sending data pull RS to HIGH (Simply uncomment below lines)*/
 
  //digitalWrite(LCD_RS, HIGH); //DATA MODE
  digitalWrite(LCD_RS, LOW);    //INSTRUCTION MODE
  data();
  digitalWrite(LCD_EN, HIGH);
  delayMicroseconds(1);
  digitalWrite(LCD_EN, LOW);
  
  if (LCD_RS == LOW)
  Serial.println("\t  Instruction Uploaded!!");
  else
  Serial.println("\t  Data Sent!!");
}

void data() {
  /* Refer ASCII table for character address*/
  digitalWrite(D0, 1); 
  digitalWrite(D1, 1);
  digitalWrite(D2, 1);
  digitalWrite(D3, 1);
  digitalWrite(D4, 0);
  digitalWrite(D5, 0);
  digitalWrite(D6, 0);
  digitalWrite(D7, 0);
}

void loop() {
  //Remains Empty
}
