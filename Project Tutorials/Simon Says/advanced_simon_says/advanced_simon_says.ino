// shift register controlled lcd library
#include <LiquidCrystal595.h>

// CONSTANTS
// data pins to lcd shift register
// connected to SER/DS - 74HC595 pin 14
const byte lcd_data = 2;
// connected to RCLK/ST_CP - 74HC595 pin 12
const byte lcd_latch = 3;
// connected to SRCLK/SH_CP - 74HC595 pin 11
const byte lcd_clock = 4;

// data pins to led shift register
// connected to SER/DS - 74HC595 pin 14
const byte led_data = 5;
// connected to RCLK/ST_CP - 74HC595 pin 12
const byte led_latch = 6;
// connected to SRCLK/SH_CP - 74HC595 pin 11
const byte led_clock = 7;

// connected to Q8 - CD4021b pin 3
const byte btn_data = 8;
// connected to CLOCK - CD4021b pin 10
const byte btn_clock = 9;
// connected to P/S C - CD4021b pin 9
const byte btn_latch = 10;

// setup buzzer pin
const byte speakerPin = 11;

// COMPONENTS
// number of options in the game
const byte num_of_options = 4;

// binary led values
const unsigned int leds [] = {1, 2, 4, 8};

// led shift register value
unsigned int led_register = 0;

// stores current button state
byte btn_state [num_of_options];

// setup tone frequency to be played by buzzer
const unsigned int notes [] = {523, 784, 830, 1397};

// counter variable
int i;

// lcd object
LiquidCrystal595 lcd(lcd_data, lcd_latch, lcd_clock);

// ROUND VARIABLES
// constant to hold the max number of rounds that can be played
const byte maxRounds = 100;

// variable to count the value of the current round
byte roundCounter = 0;

// setup array to hold the series of prompts for every round
byte promptArray [maxRounds];

void setup() {
	Serial.begin(9600);

	// setup shift register pins as outputs
	pinMode(led_data, OUTPUT);
	pinMode(led_latch, OUTPUT);
	pinMode(led_clock, OUTPUT);

	pinMode(btn_data, INPUT);
	pinMode(btn_clock, OUTPUT);
	pinMode(btn_latch, OUTPUT);

	// clear leds
	clearLed();

	// set up lcd
	lcd.begin(16,2);
	lcd.clear();

	// setup up random seed with floating analog input
	int seed = analogRead(0);
	randomSeed(seed);
}

void loop() {
	// Display initial message
	// wait for button press to play
	lcd.clear();
	lcd.setCursor(0,0);
	lcd.print("Press any button");
	lcd.setCursor(0,1);
	lcd.print("to play!");

	// wait for button and light the led of the button that was pressed.
	// then wait for the button to be depressed
	waitForBtnPress();
	setLedByBtn();
	writeRegister();
	waitForDePress();
	clearLed();
	writeRegister();

	// begin game
}

// set the desired led HIGH or LOW
void setLed(byte num, bool value) {
	// if value is HIGH, make register value 1
	// 0101 0100 | 0000 0001 = 0101 0101
	if (value) {
		led_register |= leds[num];
	}

	// if LOW, make register value 0
	// 0101 0101 & ~(0000 0001) = 0101 0101 & 1111 1110 = 0101 0100
	else {
		led_register &= ~leds[num];
	}
}

// set all registers low
void clearLed() {
	for (i = 0; i < 8; i++) {
		setLed(i, LOW);
	}
}

void setLedByBtn() {
	clearLed();
	for (i = 0; i < num_of_options; i++) {
		setLed(i, btn_state[i]);
	}
}

// writes out whatever is stored in led_register
// need to call this after any register changes
void writeRegister() {
	digitalWrite(led_latch, LOW);
	shiftOut(led_data, led_clock, MSBFIRST, led_register);
	digitalWrite(led_latch, HIGH);
}

// read button values from shift register
// store in btn_state
void btn_read() {
	// pulse latch to read pins
	digitalWrite(btn_latch, HIGH);
	delayMicroseconds(20);
	digitalWrite(btn_latch, LOW);
	byte state = shiftIn(btn_data, btn_clock);

	for (i = 0; i < num_of_options; i++) {
		if (~state & (1 << i)) {
			btn_state[i] = 1;
		} else {
			btn_state[i] = 0;
		}
	}
}

byte shiftIn(byte btn_data, byte btn_clock) {
	byte state = 0;

	// move from pin 8 to pin 1 on shift register
	// most significant bit first
	for (i = 7; i >= 0; i--) {
		// set clock low to read data pin
		digitalWrite(btn_clock, LOW);

		// check value of data pin
		if (digitalRead(btn_data)) {
			state |= (1 << i);
		}

		digitalWrite(btn_clock, HIGH);
	}

	return state;
}

void waitForBtnPress() {
	bool pressFlag = 0;

	while (not pressFlag) {
		btn_read();
		for (i = 0; i < num_of_options; i++) {
			if (btn_state[i] == 1) {
				pressFlag = 1;
				break;
			}
		}
	}
}

void waitForDePress() {
	bool pressFlag = 1;

	while (pressFlag) {
		btn_read();
		for (i = 0; i < num_of_options; i++) {
			// if every button is depressed, then pressFlag will stay false and end the while loop
			if (btn_state[i] == 0) {
				pressFlag = 0;
			} else {
				pressFlag = 1;
				// leave loop so pressFlag cannot be overwritten
				break;
			}
		}
		Serial.println("");
	}
}
