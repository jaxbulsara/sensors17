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
const byte speaker_pin = 11;

// COMPONENTS
// number of options in the game
const byte num_of_options = 4;

// binary led values
const unsigned int leds [] = {1, 2, 4, 8};

// led shift register value
unsigned int led_register = 0;

// stores current button state
byte btn_state [num_of_options];

// variable to hold which button is pressed
byte which_button;

// setup tone frequency to be played by buzzer
const unsigned int notes [] = {523, 784, 830, 1397};

// counter variable
int i;

// lcd object
LiquidCrystal595 lcd(lcd_data, lcd_latch, lcd_clock);

// stores the current highscore
unsigned int highscore = 0;

// timing variable
unsigned long startTime;

void setup() {
	// open serial channel for debugging
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
	writeRegister();

	// set up lcd
	lcd.begin(16,2);
	lcd.clear();
}

void loop() {
	// Display initial message
	// wait for button press to play
	lcd.clear();
	lcd.setCursor(0,0);
	lcd.print("Press a button.");
	lcd.setCursor(0,1);
	lcd.print("Highscore: ");
	lcd.print(highscore);

	// wait for button and light the led of the button that was pressed.
	// then wait for the button to be depressed
	waitForBtnPress();
	setLedByBtn();
	writeRegister();
	waitForDePress(0);
	clearLed();
	writeRegister();

	// display ready to play message
	lcd.clear();
	lcd.setCursor(0,0);
	lcd.print("Starting game!");

	delay(2000);

	// begin game
	playGame();
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

		delay(20);
	}
}

void waitForDePress(unsigned int timeout) {
	bool pressFlag = 1;
	unsigned long startTime = millis();

	while (pressFlag) {
		if (timeout) {
			if (millis() - startTime >= 200) {
				break;
			}
		}

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

		delay(20);
	}
}

byte checkButton() {
	// assumes the user only presses one button
	// the first pressed button from the left will be passed on
	for (i = 0; i < num_of_options; i++) {
		if (btn_state[i] == 1) {
			return i;
		}
	}

	// if no button is pressed, return number of options
	return num_of_options;
}

void playGame() {
	// setup up random seed with floating analog input
	int seed = analogRead(0);
	randomSeed(seed);

	// ROUND VARIABLES
	// constant to hold the max number of rounds that can be played
	const byte maxRounds = 100;

	// variable to count the value of the current round
	byte round_counter = 0;

	// user turn round counter
	byte user_round;

	// setup array to hold the series of prompts for every round
	byte prompt_array [maxRounds];

	// true if the user hasn't messed up
	bool play_flag = 1;

	while (play_flag) {
		// display round number and high score
		lcd.clear();
		lcd.setCursor(0,0);
		lcd.print("Round ");
		lcd.print(round_counter + 1);
		lcd.setCursor(0,1);
		lcd.print("High score: ");
		lcd.print(highscore);

		// wait a bit before displaying prompt
		delay(2000);

		// generate a random number between 0 and 3 and put it in the next available spot in the prompt array
		prompt_array[round_counter] = random(4);

		// blink each led in the sequence of prompts
		for (byte i = 0; i <= round_counter; i++) {
			// turn on the led specified by the current index of prompt_array
			setLed(prompt_array[i], HIGH);
			writeRegister();

			// stay on for a bit
			delay(200);

			// clear leds
			clearLed();
			writeRegister();

			delay(200);
		}

		// round counting variable initialization
		user_round = 0;

		// player turn
		while (1) {
			// // wait for button press
			waitForBtnPress();

			// check which button is pressed
			which_button = checkButton();

			// check if the pressed button is correct
			if (which_button != num_of_options) {
				if (which_button == prompt_array[user_round]) {
					// light led for 200 ms or until button depress
					// whichever comes first
					setLed(prompt_array[user_round], HIGH);
					writeRegister();

					// wait for button depress with timeout of 200 ms for led to turn off
					waitForDePress(200);

					clearLed();
					writeRegister();

					// wait for true button depress if still pressed
					waitForDePress(0);
				}

				// if wrong, end game
				else {
					// turn all leds on for visual cue
					for (i = 0; i < num_of_options; i++) {
						setLed(i, HIGH);
					}
					writeRegister();

					// display game over
					lcd.clear();
					lcd.setCursor(0,0);
					lcd.print("GAME OVER.");

					delay(2000);

					clearLed();
					writeRegister();

					delay(3000);

					// set play flag false so game loop ends
					play_flag = 0;
					break;
				}
			}

            //
			// if the play counter is equal to the round counter, the sequence has been completed.
			// exit the user round loop
			if (user_round == round_counter) {
				delay(1000);
				break;
			}

			user_round++;
		}

		// increment round counter
		round_counter++;

		// set new highscore if new record is set
		// round counter is one less than actual round, so this is done after the increment
		if (round_counter > highscore) {
			highscore = round_counter;
		}
	}


}
