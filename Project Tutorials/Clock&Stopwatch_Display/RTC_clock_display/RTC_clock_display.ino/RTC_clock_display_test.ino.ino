#include <SparkFunDS1307RTC.h>
#include <Wire.h>


/* -------------------- RTC CONSTANTS -------------------- */


// Comment out the line below if you want month printed before date.
// E.g. October 31, 2016: 10/31/16 vs. 31/10/16
#define PRINT_USA_DATE

#define SQW_INPUT_PIN 7   // Input pin to read SQW
#define SQW_OUTPUT_PIN 13 // LED to indicate SQW's state

/* -------------------- DISPLAY CONSTANTS -------------------- */
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

// data pins to shift register
// connected to RCLK
const byte PIN_LATCH = 9;
// connected to SRCLK
const byte PIN_CLOCK = 8;
// connected to SERz
const byte PIN_DATA = 10;

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

void setup() 
{
	// Initialize serial output
	Serial.begin(9600);

  /* -------------------- RTC SETUP -------------------- */
  pinMode(SQW_INPUT_PIN, INPUT_PULLUP);
  pinMode(SQW_OUTPUT_PIN, OUTPUT);
  digitalWrite(SQW_OUTPUT_PIN, digitalRead(SQW_INPUT_PIN));
  
  rtc.begin(); // Call rtc.begin() to initialize the library
  // (Optional) Sets the SQW output to a 1Hz square wave.
  // (Pull-up resistor is required to use the SQW pin.)
  rtc.writeSQW(SQW_SQUARE_1);

  // Now set the time...
  // You can use the autoTime() function to set the RTC's clock and
  // date to the compiliers predefined time. (It'll be a few seconds
  // behind, but close!)
  rtc.autoTime();
  // Or you can use the rtc.setTime(s, m, h, day, date, month, year)
  // function to explicitly set the time:
  // e.g. 5:39:00 | Friday November 24, 2017:
  //rtc.setTime(00, 39, 05, 5, 24, 11, 17);  // Uncomment to manually set time
  rtc.set12Hour(); // Use rtc.set12Hour to set to 12-hour mode

  /* -------------------- DISPLAY SETUP -------------------- */
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
}

void loop() 
{
  /* -------------------- RTC LOOP -------------------- */
  static int8_t lastSecond = -1;
  
  // Call rtc.update() to update all rtc.seconds(), rtc.minutes(),
  // etc. return functions.
  rtc.update();

  /* -------------------- DISPLAY LOOP -------------------- */
	// make sure the serial monitor is set to "No line endings" on the dropdown at the bottom of the window
	// if this code sees a newline or carriage return, parseInt() will convert it to a zero in the next loop.

  //Display hour.minute 
  input = rtc.hour()*100 + rtc.minute();
  //Comment above and uncomment below to display minute.second
  //input = rtc.minute()*100 + rtc.second();
  //Comment above and uncomment below to display month.day
  //input = rtc.month()*100 + rtc.day();
		
		byte i = 0;
		// place the digits of input in splitInput array in reverse order
		while (input) 
		{
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

	// display input number on display
	// the method used to fill splitInput makes it so the digits are in reverse order
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
