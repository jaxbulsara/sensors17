/******************************************************************************
Reference:
DS1307_RTC_Demo.ino
Jim Lindblom @ SparkFun Electronics
original creation date: October 2, 2016
https://github.com/sparkfun/SparkFun_DS1307_RTC_Arduino_Library

Configures, sets, and reads from the DS1307 real-time clock (RTC).

Resources:
Wire.h - Arduino I2C Library

Development environment specifics:
Arduino 1.6.8
SparkFun RedBoard
SparkFun Real Time Clock Module (v14)
******************************************************************************/
#include <SparkFunDS1307RTC.h>
#include <Wire.h>

// Comment out the line below if you want month printed before date.

/* RTC pins */
#define SQW_INPUT_PIN 6   // Input pin to read SQW
#define SQW_OUTPUT_PIN 13 // LED to indicate SQW's state

/* Button pins */
int ledPin = 7;
int buttonApin = 12;
int buttonBpin = 11;
byte leds = 0;

void setup() 
{
/* Button setup */
  pinMode(ledPin, OUTPUT);
  pinMode(buttonApin, INPUT_PULLUP);  
  pinMode(buttonBpin, INPUT_PULLUP);  

/* RTC setup */
  // Use the serial monitor to view time/date output
  Serial.begin(9600);
  
  pinMode(SQW_INPUT_PIN, INPUT_PULLUP);
  pinMode(SQW_OUTPUT_PIN, OUTPUT);
  digitalWrite(SQW_OUTPUT_PIN, digitalRead(SQW_INPUT_PIN));
    
    rtc.begin(); // Call rtc.begin() to initialize the library
    // (Optional) Sets the SQW output to a 1Hz square wave.
    // (Pull-up resistor is required to use the SQW pin.)
    rtc.writeSQW(SQW_SQUARE_1);
}

void loop() 
{
  static int8_t lastSecond = -1;

  if (digitalRead(buttonApin) == LOW)
  {
    digitalWrite(ledPin, HIGH);
 
    // Use the rtc.setTime(s, m, h, day, date, month, year)
    // function to explicitly set the time:
    // set time to 00:00:00 | Sunday January 1, 2001:
    rtc.setTime(0, 0, 0, 1, 1, 1, 01);
    
    while (digitalRead(buttonBpin) != LOW)
    {
      // Call rtc.update() to update all rtc.seconds(), rtc.minutes(),
      // etc. return functions.
      rtc.update();
    
      if (rtc.second() != lastSecond) // If the second has changed
      {
        printTime(); // Print the new time
        
        lastSecond = rtc.second(); // Update lastSecond value
      }
    
      // Read the state of the SQW pin and show it on the
      // pin 13 LED. (It should blink at 1Hz.)
      digitalWrite(SQW_OUTPUT_PIN, digitalRead(SQW_INPUT_PIN));
    }
    digitalWrite(ledPin, LOW);
  }
  
}

void printTime()
{
/*
  Serial.print(String(rtc.hour()) + ":"); // Print hour
  if (rtc.minute() < 10)
  Serial.print('0'); // Print leading '0' for minute
*/
  Serial.print(String(rtc.minute()) + ":"); // Print minute
  if (rtc.second() < 10)
    Serial.print('0'); // Print leading '0' for second
  Serial.print(String(rtc.second())); // Print second
  Serial.print("\n");
/*
  if (rtc.is12Hour()) // If we're in 12-hour mode
  {
    // Use rtc.pm() to read the AM/PM state of the hour
    if (rtc.pm()) Serial.print(" PM"); // Returns true if PM
    else Serial.print(" AM");
  }  
  Serial.print(" | ");

  // Few options for printing the day, pick one:
  Serial.print(rtc.dayStr()); // Print day string
  //Serial.print(rtc.dayC()); // Print day character
  //Serial.print(rtc.day()); // Print day integer (1-7, Sun-Sat)
  Serial.print(" - ");
#ifdef PRINT_USA_DATE
  Serial.print(String(rtc.month()) + "/" +   // Print month
                 String(rtc.date()) + "/");  // Print date
#else
  Serial.print(String(rtc.date()) + "/" +    // (or) print date
                 String(rtc.month()) + "/"); // Print month
#endif
  Serial.println(String(rtc.year()));        // Print year
*/
}
