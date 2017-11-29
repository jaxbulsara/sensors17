// Define constants - these values don't change throughout the code
// the pins that will control the leds
const byte ledPin [] = {2, 3, 4, 5};

// the pins that will read input from the buttons
const byte buttonPin [] = {6, 7, 8, 9};

const short numberOfOptions = sizeof(ledPin) / sizeof(byte);

// counter variable
int i;

// variable to hold which button is pressed
byte whichButton;

// setup function - runs once
void setup() {
	// setup pin modes as input or output

	// loop through each led pin and set up as output
	for (i = 0; i < numberOfOptions; i++) {
		pinMode(ledPin[i], OUTPUT);
		// keep led off by default
		digitalWrite(ledPin[i], LOW);
	}

	// loop through each button and set up as input
	// button pin is floating value. It relies on the pull up resistor for it's value
	// unpressed - HIGH
	// pressed - LOW
	for (i = 0; i < numberOfOptions; i++) {
		pinMode(buttonPin, INPUT);
	}

	// keep led off by default
	digitalWrite(ledPin, LOW);

	// start serial monitor for debugging
	Serial.begin(9600);

	// setup up random seed with floating analog input
	int seed = analogRead(0);
	randomSeed(seed);

	// print a debug statement
	Serial.println("Setup completed successfully.");
}

// loop function - runs continuously after setup() is run
void loop() {
	Serial.println("Blink random led.");

	// generate a random number between 0 and 3
	int prompt = random(4);

	digitalWrite(ledPin[prompt], HIGH);

	// keep led on for 300 milliseconds
	delay(300);

	digitalWrite(ledPin[prompt], LOW);

	Serial.println("Wait for correct button press.");

	// a loop to check the value of the button until it turns low
	while(1) {
		// check if a button is pressed and which one
		whichButton = checkButton();

		// check if a button has been pressed
		if (whichButton != 4) {
			// if a button has been pressed, check if it's the same as the prompt
			if (whichButton == prompt) {
				digitalWrite(ledPin[prompt], HIGH);

				// keep led on for 300 milliseconds
				delay(300);

				digitalWrite(ledPin[prompt], LOW);

				// wait for button to depress
				while (whichButton != 4) {
					whichButton = checkButton();
				}

				// leave while loop
				break;
			}
		}

		delay(20);
	}

	Serial.println("Button pressed!\n");

	delay(500);
}

// checks to see if the value of each button pin is LOW (pressed)
// if so, the function returns a value between 0 and 3 corresponding to that button
// if the loop completes and every button pin reads HIGH, the function returns 4
byte checkButton () {
	for (byte i = 0; i < 4; i++) {
		if (digitalRead(buttonPin[i]) == LOW) { return i; }
	}
	return 4;
}
