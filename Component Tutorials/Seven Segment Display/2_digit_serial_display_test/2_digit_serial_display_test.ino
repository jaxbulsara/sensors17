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

// CONSTANTS
// data pins to shift register
// connected to RCLK
const byte PIN_LATCH = 11;
// connected to SRCLK
const byte PIN_CLOCK = 12;
// connected to SER
const byte PIN_DATA = 8;

// digit on/off outputs
// connected to digit 1
const byte PIN_DIG_1 = 2;
// connected to digit 2
const byte PIN_DIG_2 = 4;

// GLOBAL DATA
// represents led configuration
byte val = 1;
// represents direction of led animation
byte dir_forward = 1;

// delay between loops
int delay_time = 500;

void setup() {
	Serial.begin(9600);

	// set pins to output so you can control the shift register
	pinMode(PIN_LATCH, OUTPUT);
	pinMode(PIN_CLOCK, OUTPUT);
	pinMode(PIN_DATA, OUTPUT);

	// the LEDs don't change while you're sending in bits
	digitalWrite(PIN_LATCH, LOW);
	// shift out the bits
	shiftOut(PIN_DATA, PIN_CLOCK, MSBFIRST, 0);
	// set latch high so LEDs will enable
	digitalWrite(PIN_LATCH, HIGH);

	// setup digit enable pins to output so you can control which digit is on
	pinMode(PIN_DIG_1, OUTPUT);
	pinMode(PIN_DIG_2, OUTPUT);

	// set both digits to be on
	digitalWrite(PIN_DIG_1, LOW);
	digitalWrite(PIN_DIG_2, LOW);
}

void loop() {
	// set latch LOW so LEDs don't change when sending in bits
	digitalWrite(PIN_LATCH, LOW);

	// send the bits
	shiftOut(PIN_DATA, PIN_CLOCK, MSBFIRST, val);

	// set next LED configuration
	if (dir_forward) {
		// val = val / 2 OR move 1 bit left
		val <<= 1;

		// 128 is 1000 0000, reached last led -> revert the sequence
		if (val == 128) {
			dir_forward = 0;
		}
	} else {
		// val = val / 2 OR move 1 bit right
		val >>= 1;

		// 1 is 0000 0001, reached first led -> revert the sequence
		if (val == 1) {
			dir_forward = 1;
		}
	}

	// set latch high so LEDs will enable
	digitalWrite(PIN_LATCH, HIGH);

	delay(delay_time);
}
