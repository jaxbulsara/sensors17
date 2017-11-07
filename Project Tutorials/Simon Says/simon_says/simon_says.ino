// define variables and constants

// pins used to control leds
const byte ledPins [] = {2, 3, 4, 5};

// pins used to read state of buttons
const byte buttons [] = {6, 7, 8, 9};

// setup buzzer pin
// use beep() function to play sounds
const byte speakerPin = 10;

// setup tone frequency to be played by buzzer
const unsigned int notes [] = {523, 784, 830, 1397};

// setup rounds
const byte maxRounds = 100;
byte roundCounter = 0;

// setup arrays for gameplay
byte promptArray [maxRounds];

// speed multiplier
byte speedMultiplier = 1;

void setup() {
	// setup serial output
	Serial.begin(9600);

	// setup random seed
	int seed = analogRead(0) * analogRead(1) * analogRead(2);
	// Serial.print("Seed is ");
	// Serial.print(seed);
	// Serial.println();
	randomSeed(seed);

	// setup LEDs: pin --> LED (long pin) --> LED (short pin) --> GND
	// on - HIGH
	// off - LOW
	for (byte i = 0; i < 4; i++) {
		pinMode(ledPins[i], OUTPUT);
		digitalWrite(ledPins[i], LOW);
	}

	// setup buttons: pin --> button --> GND
	// buttons set up with pull up resistor
	// pressed - LOW
	// unpressed - HIGH
	for (byte i = 0; i < 4; i++) {
		pinMode(buttons[i], INPUT_PULLUP);
	}

	Serial.println("Start game.");

	for (byte i = 0; i < 4; i++) {
		userPress(i, true);
	}
	for (int i = 2; i >= 0; i--) {
		userPress(i, true);
	}
	delay(1000);
}

void loop() {
	// if max number of rounds reached, end program
	if (roundCounter == maxRounds) {
		Serial.println();
		exit(0);
	}

	// generate random number between 0 and 3
	promptArray[roundCounter] = random(4);

	// blink the leds and play sounds from promtArray
	displayPrompt();

	delay(100);

	userTurn();

	// sleep(500);

	// DEBUG only play last round
	// if (roundCounter == maxRounds - 1) { displayPrompt(); }

	roundCounter++;
}

void displayPrompt() {
	Serial.print("Displaying prompt for round ");
	Serial.print(roundCounter);
	Serial.println();
	for (unsigned int i = 0; i <= roundCounter; i++ ) {
		// Serial.print(promptArray[i]);
		// Serial.print(" ");
		digitalWrite(ledPins[promptArray[i]], HIGH);
		tone(speakerPin, notes[promptArray[i]]);
		sleep(200);
		digitalWrite(ledPins[promptArray[i]], LOW);
		noTone(speakerPin);
		sleep(100);
	}

	// Serial.println();
}

void userPress(byte button, bool right) {
	if (right) {
		digitalWrite(ledPins[button], HIGH);
		tone(speakerPin, notes[button]);
		sleep(200);
		digitalWrite(ledPins[button], LOW);
		noTone(speakerPin);
		sleep(10);
	} else {
		for (byte i = 0; i < 3; i++) {
			digitalWrite(ledPins[button], HIGH);
			tone(speakerPin, 440);
			sleep(200);
			digitalWrite(ledPins[button], LOW);
			noTone(speakerPin);
			sleep(50);
		}
	}
}

void userTurn( void ) {
	// user presses buttons that match the order of promtArray
	bool playFlag = true;
	byte button = 4;
	byte userRound = 0;

	Serial.println("Begin user turn.");

	while (playFlag) {
		// check if user has pressed a button
		button = checkButton();

		// if pressed, check value and continue
		if (button != 4) {
			Serial.println(button);
			if (button == promptArray[userRound]) {
				userPress(button, true);
			} else {
				userPress(button, false);
				exit(0);
			}

			// move to next round
			userRound++;

			// wait for user to release button
			while (button != 4) {
				button = checkButton();
			}

			delay(10);
		}

		if (userRound == roundCounter + 1) {playFlag = false; delay(1000);}

		delay(10);
	}
}

byte checkButton () {
	for (byte i = 0; i < 4; i++) {
		if (not digitalRead(buttons[i])) { return i; }
	}
	return 4;
}

void sleep(unsigned int dur) {
	dur = dur / speedMultiplier;
	delay(dur);
}
