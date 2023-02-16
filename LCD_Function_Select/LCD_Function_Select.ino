#include <Arduino.h>
#include <A4988.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define clk 2
#define dt 3
#define sw 4
#define limit1 6
#define limit2 7
A4988 stepper(200, 8, 9, 10, 11, 12);


volatile boolean TurnDetected;
volatile boolean up;
int dir;
int arrow;
int mode;
int rpm;
int minutes;
int minrpm;

LiquidCrystal_I2C lcd(0x27, 16, 2);

byte customChar1[8] = {
  0b10000,
  0b11000,
  0b11100,
  0b11110,
  0b11110,
  0b11100,
  0b11000,
  0b10000
};

byte customChar2[8] = {
  0b00100,
  0b01110,
  0b11111,
  0b00000,
  0b00000,
  0b11111,
  0b01110,
  0b00100,
};


void isr0 ()  {
  TurnDetected = true;
  up = (digitalRead(clk) == digitalRead(dt));
}

void setup() {
  lcd.init();
  lcd.backlight();
  lcd.begin(16, 2);
  lcd.createChar(0, customChar1);
  lcd.createChar(1, customChar2);
  pinMode(clk, INPUT);
  pinMode(dt, INPUT);
  pinMode(sw, INPUT);
  pinMode(limit1, INPUT);
  pinMode(limit2, INPUT);
  attachInterrupt (0, isr0, RISING);
  mode = 0;
  arrow = 0;
  rpm = 100;
  dir = 0;
  minutes = 1;
  minrpm = 100;
  stepper.setMicrostep(8);
}

void loop() {

  if (mode == 0) {  // Home Screen

    if (TurnDetected) {
      lcd.clear();
      if (up) {
        arrow = !arrow;
      }
      else {
        arrow = !arrow;
      }
      TurnDetected = false;
    }

    lcd.setCursor(1, 0);
    lcd.print("RPM Mode");
    lcd.setCursor(1, 1);
    lcd.print("Time Mode");
    if (arrow == 0) {
      lcd.setCursor(0, 0);
      lcd.write((uint8_t)0);
    }
    else {
      lcd.setCursor(0, 1);
      lcd.write((uint8_t)0);
    }
  }

  if (!(digitalRead(sw))) {
    lcd.clear();
    if (arrow == 0) { // RPM Mode
      mode = 1;
      delay(200);
    }
    else {            // Time Mode
      mode = 2;
      delay(200);
    }
    arrow = 0;
  }

  if (mode == 1) {  // RPM Screen

    if (TurnDetected) {
      lcd.clear();
      if (up) {
        arrow = arrow + 1;
        if (arrow > 2) {
          arrow = 0;
        }
      }
      else {
        arrow = arrow - 1;
        if (arrow < 0) {
          arrow = 2;
        }
      }
      TurnDetected = false;
    }

    lcd.setCursor(1, 0);
    lcd.print("RPM:");
    lcd.print(rpm);
    lcd.setCursor(1, 1);
    lcd.print("Start");
    lcd.setCursor(8, 1);
    lcd.print("Back");
    if (arrow == 0) {
      lcd.setCursor(0, 0);
      lcd.write((uint8_t)0);
    }
    if (arrow == 1) {
      lcd.setCursor(0, 1);
      lcd.write((uint8_t)0);
    }
    if (arrow == 2) {
      lcd.setCursor(7, 1);
      lcd.write((uint8_t)0);
    }

    if (!(digitalRead(sw))) {
      if (arrow == 0) {
        lcd.clear();
        mode = 11;
        delay(200);
      }
      if (arrow == 1) {
        lcd.clear();
        mode = 12;
        delay(200);
      }
      if (arrow == 2) {
        lcd.clear();
        rpm = 100;
        mode = 0;
        delay(200);
      }
      arrow = 0;
    }
  }

  if (mode == 11) {  // Choose RPM, Motor Off

    if (TurnDetected) {
      lcd.clear();
      if (up) {
        if (rpm < 20) {
          rpm = rpm + 1;
        }
        else {
          rpm = rpm + 5;
        }
        if (rpm >= 400) {
          rpm = 400;
        }
      }
      else {
        if (rpm <= 20) {
          rpm = rpm - 1;
        }
        else {
          rpm = rpm - 5;
        }
        if (rpm <= 1) {
          rpm = 1;
        }
      }
      TurnDetected = false;
    }

    lcd.setCursor(1, 0);
    lcd.print("RPM:");
    lcd.print(rpm);
    lcd.setCursor(1, 1);
    lcd.print("Start");
    lcd.setCursor(8, 1);
    lcd.print("Back");
    lcd.setCursor(9, 0);
    lcd.write((uint8_t)1);

    if (!(digitalRead(sw))) {
      lcd.clear();
      mode = 1;
      delay(200);
    }
  }

  if (mode == 12) {  // Motor ON

    if (TurnDetected) {
      lcd.clear();
      if (up) {
        arrow = !arrow;
      }
      else {
        arrow = !arrow;
      }
      TurnDetected = false;
    }

    lcd.setCursor(1, 0);
    lcd.print("RPM:");
    lcd.print(rpm);
    lcd.setCursor(1, 1);
    lcd.print("Stop");
    if (arrow == 0) {
      lcd.setCursor(0, 0);
      lcd.write((uint8_t)0);
    }
    if (arrow == 1) {
      lcd.setCursor(0, 1);
      lcd.write((uint8_t)0);
    }

    while (!TurnDetected && (digitalRead(sw)) && (digitalRead(limit1)) && (digitalRead(limit2))) {
      stepper.setRPM(rpm);
      if (dir == 0) {
        stepper.rotate(-1);
      }
      else {
        stepper.rotate(1);
      }
    }

    if (!(digitalRead(limit1))) {
      dir = 0;
      stepper.rotate(-1);
    }

    if (!(digitalRead(limit2))) {
      dir = 1;
      stepper.rotate(1);
    }

    if (!(digitalRead(sw))) {
      if (arrow == 0) {
        lcd.clear();
        mode = 13;
        delay(200);
      }
      if (arrow == 1) {
        lcd.clear();
        arrow = 0;
        mode = 1;
        stepper.disable();
        delay(200);
      }
    }
  }

  if (mode == 13) {    // Choose RPM, Motor ON

    if (TurnDetected) {
      lcd.clear();
      if (up) {
        if (rpm < 20) {
          rpm = rpm + 1;
        }
        else {
          rpm = rpm + 5;
        }
        if (rpm >= 400) {
          rpm = 400;
        }
      }
      else {
        if (rpm <= 20) {
          rpm = rpm - 1;
        }
        else {
          rpm = rpm - 5;
        }
        if (rpm <= 1) {
          rpm = 1;
        }
      }
      TurnDetected = false;
    }

    lcd.setCursor(1, 0);
    lcd.print("RPM:");
    lcd.print(rpm);
    lcd.setCursor(1, 1);
    lcd.print("Stop");
    lcd.setCursor(9, 0);
    lcd.write((uint8_t)1);

    while (!TurnDetected && (digitalRead(sw)) && (digitalRead(limit1)) && (digitalRead(limit2))) {
      stepper.setRPM(rpm);
      if (dir == 0) {
        stepper.rotate(-1);
      }
      else {
        stepper.rotate(1);
      }
    }

    if (!(digitalRead(limit1))) {
      dir = 0;
      stepper.rotate(-1);
    }

    if (!(digitalRead(limit2))) {
      dir = 1;
      stepper.rotate(1);
    }

    if (!(digitalRead(sw))) {
      lcd.clear();
      mode = 12;
      delay(200);
    }
  }

  if (mode == 2) {  // Time Screen

    if (TurnDetected) {
      lcd.clear();
      if (up) {
        arrow = arrow + 1;
        if (arrow > 2) {
          arrow = 0;
        }
      }
      else {
        arrow = arrow - 1;
        if (arrow < 0) {
          arrow = 2;
        }
      }
      TurnDetected = false;
    }

    lcd.setCursor(1, 0);
    lcd.print("Time:");
    lcd.print(minutes);
    lcd.setCursor(8, 0);
    lcd.print("min");
    lcd.setCursor(1, 1);
    lcd.print("Start");
    lcd.setCursor(8, 1);
    lcd.print("Back");
    if (arrow == 0) {
      lcd.setCursor(0, 0);
      lcd.write((uint8_t)0);
    }
    if (arrow == 1) {
      lcd.setCursor(0, 1);
      lcd.write((uint8_t)0);
    }
    if (arrow == 2) {
      lcd.setCursor(7, 1);
      lcd.write((uint8_t)0);
    }

    if (!(digitalRead(sw))) {
      if (arrow == 0) {
        lcd.clear();
        mode = 21;
        delay(200);
      }
      if (arrow == 1) {
        lcd.clear();
        mode = 22;
        delay(200);
      }
      if (arrow == 2) {
        lcd.clear();
        minutes = 1;
        mode = 0;
        delay(200);
      }
      arrow = 0;
    }
  }

  if (mode == 21) {  // Choose Time, Motor Off

    if (TurnDetected) {
      lcd.clear();
      if (up) {
        if (minutes < 20) {
          minutes = minutes + 1;
        }
        else {
          minutes = 20;
        }
      }
      else {
        if ( minutes > 1) {
          minutes = minutes - 1;
        }
        else {
          minutes = 1;
        }
      }
      TurnDetected = false;
    }

    lcd.setCursor(1, 0);
    lcd.print("Time:");
    lcd.print(minutes);
    lcd.setCursor(8, 0);
    lcd.print("min");
    lcd.setCursor(1, 1);
    lcd.print("Start");
    lcd.setCursor(8, 1);
    lcd.print("Back");
    lcd.setCursor(12, 0);
    lcd.write((uint8_t)1);

    if (!(digitalRead(sw))) {
      lcd.clear();
      mode = 2;
      delay(200);
    }
  }

  if (mode == 22) {  // Right to Left, Left to Right

    if (TurnDetected) {
      lcd.clear();
      if (up) {
        arrow = !arrow;
      }
      else {
        arrow = !arrow;
      }
      TurnDetected = false;
    }

    lcd.setCursor(1, 0);
    lcd.print("Right to Left");
    lcd.setCursor(1, 1);
    lcd.print("Left to Right");
    if (arrow == 0) {
      lcd.setCursor(0, 0);
      lcd.write((uint8_t)0);
    }
    if (arrow == 1) {
      lcd.setCursor(0, 1);
      lcd.write((uint8_t)0);
    }

    if (!(digitalRead(sw))) {
      if (arrow == 0) {
        lcd.clear();
        mode = 24;
        delay(200);
      }
      if (arrow == 1) {
        lcd.clear();
        arrow = 0;
        mode = 23;
        delay(200);
      }
      arrow = 0;
    }
  }

  if (mode == 23) {  // Right to Left, Motor ON

    lcd.setCursor(1, 0);
    lcd.print("Time:");
    lcd.print(minutes);
    lcd.setCursor(8, 0);
    lcd.print("min");
    lcd.setCursor(1, 1);
    lcd.print("Stop");
    if (arrow == 0) {
      lcd.setCursor(0, 1);
      lcd.write((uint8_t)0);
    }
    while ((digitalRead(sw)) && (digitalRead(limit1))) {
      stepper.setRPM(100);
      stepper.rotate(1);
    }

    if (!(digitalRead(limit1))) {
      while ((digitalRead(limit2)) && (digitalRead(sw))) {
        minrpm = round(20/minutes);
        stepper.setRPM(minrpm);
        stepper.rotate(-1);
      }
    }

    if (!(digitalRead(limit2))) {
      lcd.clear();
      mode = 0;
      minutes = 1;
      stepper.disable();
    }


    if (!(digitalRead(sw))) {
      lcd.clear();
      mode = 2;
      delay(200);
    }
  }

  if (mode == 24) {  // Left to Right, Motor ON

    lcd.setCursor(1, 0);
    lcd.print("Time:");
    lcd.print(minutes);
    lcd.setCursor(8, 0);
    lcd.print("min");
    lcd.setCursor(1, 1);
    lcd.print("Stop");
    if (arrow == 0) {
      lcd.setCursor(0, 1);
      lcd.write((uint8_t)0);
    }
    while ((digitalRead(sw)) && (digitalRead(limit2))) {
      stepper.setRPM(100);
      stepper.rotate(-1);
    }

    if (!(digitalRead(limit2))) {
      while ((digitalRead(limit1)) && (digitalRead(sw))) {
        minrpm = round(20/minutes);
        stepper.setRPM(minrpm);
        stepper.rotate(1);
      }
    }

    if (!(digitalRead(limit1))) {
      lcd.clear();
      mode = 0;
      minutes = 1;
      stepper.disable();
    }


    if (!(digitalRead(sw))) {
      lcd.clear();
      mode = 2;
      delay(200);
    }
  }

}
