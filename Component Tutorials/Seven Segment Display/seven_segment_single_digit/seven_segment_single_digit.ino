// WIRING
// PIN	SSD	SEG
// 6	7	A
// 7	6	B
// 8	4	C
// 9	2	D
// 10	1	E
// 11	9	F
// 12	10	G
// 13	5	DP

// NUMBER CODES
// These codes correspond to a COMMON CATHODE display - 1 being ON and 0 being OFF.
// num	   a   b   c   d   e   f   g   dp
// 0       1   1   1   1   1   1   0   0
// 1       0   1   1   0   0   0   0   0
// 2       1   1   0   1   1   0   1   0
// 3       1   1   1   1   0   0   1   0
// 4       0   1   1   0   0   1   1   0
// 5       1   0   1   1   0   1   1   0
// 6       1   0   1   1   1   1   1   0
// 7       1   1   1   0   0   1   0   0
// 8       1   1   1   1   1   1   1   0
// 9       1   1   1   1   0   1   1   0

// These codes correspond to a COMMON ANODE display - 1 being OFF and 0 being ON.
// num	   a   b   c   d   e   f   g   dp
// 0       0   0   0   0   0   0   1   1
// 1       1   0   0   1   1   1   1   1
// 2       0   0   1   0   0   1   0   1
// 3       0   0   0   0   1   1   0   1
// 4       1   0   0   1   1   0   0   1
// 5       0   1   0   0   1   0   0   1
// 6       0   1   0   0   0   0   0   1
// 7       0   0   0   1   1   0   1   1
// 8       0   0   0   0   0   0   0   1
// 9       0   0   0   0   1   0   0   1

// CONSTANTS
// use this value of digitCodes if you have a common cathode display
const byte digit_pins [] = {6, 7, 8, 9, 10, 11, 12};
const byte decimal_pin = 13;

// use this array of digit codes if you have a COMMON CATHODE display. Comment out the other array.
const byte digit_codes [][7] = {
	{1, 1, 1, 1, 1, 1, 0}, // 0
	{0, 1, 1, 0, 0, 0, 0}, // 1
	{1, 1, 0, 1, 1, 0, 1}, // 2
	{1, 1, 1, 1, 0, 0, 1}, // 3
	{0, 1, 1, 0, 0, 1, 1}, // 4
	{1, 0, 1, 1, 0, 1, 1}, // 5
	{1, 0, 1, 1, 1, 1, 1}, // 6
	{1, 1, 1, 0, 0, 1, 0}, // 7
	{1, 1, 1, 1, 1, 1, 1}, // 8
	{1, 1, 1, 1, 0, 1, 1}, // 9
};

const byte decimal_code = 1;

// use this array of digit codes if you have a COMMON ANODE display. Comment out the other array.
/*
const byte digit_codes [][7] = {
	{0, 0, 0, 0, 0, 0, 1}, // 0
	{1, 0, 0, 1, 1, 1, 1}, // 1
	{0, 0, 1, 0, 0, 1, 0}, // 2
	{0, 0, 0, 0, 1, 1, 0}, // 3
	{1, 0, 0, 1, 1, 0, 0}, // 4
	{0, 1, 0, 0, 1, 0, 0}, // 5
	{0, 1, 0, 0, 0, 0, 0}, // 6
	{0, 0, 0, 1, 1, 0, 1}, // 7
	{0, 0, 0, 0, 0, 0, 0}, // 8
	{0, 0, 0, 0, 1, 0, 0}, // 9
};

const byte decimal_code = 0;
*/

// VARIABLES
// flag to toggle decimal point
byte decimal_flag = LOW;

// counter variables
int i, j;

void setup() {
	// loop through digit_pins array and set each pin to be an output
	// set each LOW to turn LED off
	for (i = 0; i < 7; i++) {
		pinMode(digit_pins[i], OUTPUT);
		digitalWrite(digit_pins[i], LOW);
	}

	// set decimal pin to be an output
	// set LOW to turn LED off
	pinMode(decimal_pin, OUTPUT);
	digitalWrite(decimal_pin, LOW);
}

void loop() {
	// Display each number without a decimal point
	// Then display each number with a decimal point

	// turn on decimal point based on value of decimal_flag, which flips at the end of each iteration of loop()
	digitalWrite(decimal_pin, decimal_flag);

	// for the ten digits, 0-9, which correspond to the rows of digit_codes
	for (i = 0; i< 10; i++) {
		// for the 7 segments, turn on the appropriate segments based on the columns of digit_codes
		for (j = 0; j < 7; j++) {
			digitalWrite(digit_pins[j], digit_codes[i][j]);
		}

		// keep digit on for a little while
		delay(500);

		// turn the segments off and move to the next number.
		for (j = 0; j < 7; j++) {
			digitalWrite(digit_pins[j], LOW);
		}
	}

	// flip the value of decimal flag so the decimal point will show/not show during the next loop
	decimal_flag = !decimal_flag;
}
