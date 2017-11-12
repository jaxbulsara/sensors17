#include <Wire.h>
#include <SparkFunDS1307RTC.h>

// WIRING
// SR   7SD   SEG   NUM
// A -> 1  -> A  -> 1
// B -> 2  -> B  -> 2
// C -> 4  -> C  -> 4
// D -> 5  -> D  -> 8
// E -> 10 -> E  -> 16
// F -> 9  -> F  -> 32
// G -> 7  -> G  -> 64
// H -> 6  -> DP -> 128

// add the segment NUM to 0 to get the serial value for the number codes below

// NUMBER CODES
// These codes correspond to a COMMON CATHODE display - 1 being ON and 0 being OFF.
// num     a   b   c   d   e   f   g   dp  val
// 0       1   1   1   1   1   1   0   0   63
// 1       0   1   1   0   0   0   0   0   6
// 2       1   1   0   1   1   0   1   0   91
// 3       1   1   1   1   0   0   1   0   79
// 4       0   1   1   0   0   1   1   0   102
// 5       1   0   1   1   0   1   1   0   109
// 6       1   0   1   1   1   1   1   0   125
// 7       1   1   1   0   0   1   0   0   39
// 8       1   1   1   1   1   1   1   0   127
// 9       1   1   1   1   0   1   1   0   111

// These codes correspond to a COMMON ANODE display - 1 being OFF and 0 being ON.
// num     a   b   c   d   e   f   g   dp  val
// 0       0   0   0   0   0   0   1   1   192
// 1       1   0   0   1   1   1   1   1   249
// 2       0   0   1   0   0   1   0   1   164
// 3       0   0   0   0   1   1   0   1   176
// 4       1   0   0   1   1   0   0   1   153
// 5       0   1   0   0   1   0   0   1   146
// 6       0   1   0   0   0   0   0   1   130
// 7       0   0   0   1   1   0   1   1   216
// 8       0   0   0   0   0   0   0   1   128
// 9       0   0   0   0   1   0   0   1   144
//----------------------------------------------------------//

/* ----------------- RTC pins ----------------- */
#define SQW_INPUT_PIN 6   // Input pin to read SQW
#define SQW_OUTPUT_PIN 13 // LED to indicate SQW's state

/* ----------------- Button pins ----------------- */
int ledPin = 7;
int buttonApin = 12;
int buttonBpin = 11;
byte leds = 0;

/* ----------------- Display pins & constants ----------------- */
// data pins to shift register
// connected to RCLK
const byte PIN_LATCH = 9;
// connected to SRCLK
const byte PIN_CLOCK = 10;
// connected to SERz
const byte PIN_DATA = 8;

// digit enable pins - 1 is leftmost digit
// connected to digit 1
const byte PIN_DIG_1 = 2;
// connected to digit 2
const byte PIN_DIG_2 = 3;
// connected to digit 3
const byte PIN_DIG_3 = 4;
// connected to digit 4
const byte PIN_DIG_4 = 5;

// these numbers, converted to binary, represent which segments need to be lit up to make the number
// each code corresponds to the number of its index (position)
// use this value of digitCodes if you have a COMMON CATHODE display
//                          0   1  2   3   4    5    6    7   8    9
const byte digitCodes [] = {63, 6, 91, 79, 102, 109, 125, 39, 127, 111};

// Comment the above and uncomment below if you have a COMMON ANODE display
//                          0    1    2    3    4    5    6    7    8    9
// const byte digitCodes [] = {192, 249, 164, 176, 153, 146, 130, 216, 128, 144};

// the digit pins must be in an array in order to loop through them
// the digits need to be ordered from the rightmost digit to the leftmost to display the number correctly
const byte digitPins [] = {PIN_DIG_4, PIN_DIG_3, PIN_DIG_2, PIN_DIG_1};
const byte numberOfDigits = sizeof(digitPins) / sizeof(byte);

// maxNumber is the first number with more digits than numberOfDigits
// it is used to check that the input number is valid
const int maxNumber = pow(10, numberOfDigits);

// VARIABLES
// stores input number from serial input
int input;

// stores the input as an array of numbers for display loop
byte splitInput [numberOfDigits];

// timing variables for display loop
unsigned long currentMillis;
unsigned int delay_time = 1;

void setup() {
/* ----------------- Button setup ----------------- */
  pinMode(ledPin, OUTPUT);
  pinMode(buttonApin, INPUT_PULLUP);  
  pinMode(buttonBpin, INPUT_PULLUP);  

/* ----------------- RTC setup ----------------- */
  // Use the serial monitor to view time/date output
  Serial.begin(9600);
  
  pinMode(SQW_INPUT_PIN, INPUT_PULLUP);
  pinMode(SQW_OUTPUT_PIN, OUTPUT);
  digitalWrite(SQW_OUTPUT_PIN, digitalRead(SQW_INPUT_PIN));
    
    rtc.begin(); // Call rtc.begin() to initialize the library
    // (Optional) Sets the SQW output to a 1Hz square wave.
    // (Pull-up resistor is required to use the SQW pin.)
    rtc.writeSQW(SQW_SQUARE_1);
    
/* ----------------- Display Setup ----------------- */
  // Initialize serial output
  Serial.begin(9600);

  // set shift register pins to output
  pinMode(PIN_LATCH, OUTPUT);
  pinMode(PIN_CLOCK, OUTPUT);
  pinMode(PIN_DATA, OUTPUT);

  // setup digit enable pins to output so you can control which digit is on
  pinMode(PIN_DIG_1, OUTPUT);
  pinMode(PIN_DIG_2, OUTPUT);
  pinMode(PIN_DIG_3, OUTPUT);
  pinMode(PIN_DIG_4, OUTPUT);

  // the LEDs don't change while you're sending in bits
  digitalWrite(PIN_LATCH, LOW);
  // shift out the bits
  shiftOut(PIN_DATA, PIN_CLOCK, MSBFIRST, 0);
  // set latch high so LEDs will enable
  digitalWrite(PIN_LATCH, HIGH);

  // set digits to be off
  // this code assumes an PNP transistor is used
  // ON when the base is LOW
  // OFF when the base is HIGH
  digitalWrite(PIN_DIG_1, HIGH);
  digitalWrite(PIN_DIG_2, HIGH);
  digitalWrite(PIN_DIG_3, HIGH);
  digitalWrite(PIN_DIG_4, HIGH);

  Serial.print("Setup complete. \n Press button A to begin.");
}

void loop() 
{
/* ----------------- Timer loop ----------------- */
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

/* ----------------- Display loop ----------------- */
  // make sure the serial monitor is set to "No line endings" on the dropdown at the bottom of the window
  // if this code sees a newline or carriage return, parseInt() will convert it to a zero in the next loop.
  if (Serial.available()) 
  {
    input = Serial.parseInt();

    // if the parsed integer contains more digits than can be displayed, alert the user.
    if (input >= maxNumber) 
    {
      Serial.print("Number cannot be longer than ");
      Serial.print(numberOfDigits);
      Serial.println(" digits long");
    } 
    else 
    {
      // the number is an appropriate length, so we can begin putting it into splitInput that we can hand to the display loop
      Serial.print("Displaying: ");
      Serial.println(input);

      byte i = 0;

      // place the digits of input in splitInput array in reverse order
      while (input) {
        // this loop takes the remainder of the input string when divided by 10
        // this will always be the last digit
        splitInput[i] = input % 10;

        // then divide by 10
        // because input is an integer, the decimal will be cut off and in the next loop, the next number will be input into splitInput
        input /= 10;
        i++;
      }

      // if the above loop terminates while there are still digits to fill in splitInput, keep adding zeros to it until it's full.
      while (i < numberOfDigits) 
      {
        splitInput[i] = 0;
        i++;
      }
    }
  }

  // display input number on display
  // the method used to fill splitInput makes it so the  digits are in reverse order
  // this is corrected by placing the digit pin numbers in digitPins from the rightmost digit to the left.
  for (byte i = 0; i < numberOfDigits; i++) {
    // set latch LOW so LEDs don't change when sending in bits
    digitalWrite(PIN_LATCH, LOW);

    // send the bits
    shiftOut(PIN_DATA, PIN_CLOCK, MSBFIRST, digitCodes[splitInput[i]]);

    // turn digit on
    digitalWrite(digitPins[i], LOW);

    // set latch high so LEDs will enable
    digitalWrite(PIN_LATCH, HIGH);

    currentMillis = millis();
    while (millis() - currentMillis < delay_time);

    // turn digit off
    digitalWrite(digitPins[i], HIGH);
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
}
