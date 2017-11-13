// Define constants - these values don't change throughout the code
// the pins that will control the leds
const byte ledPin [] = {2, 3, 10, 5};

// the pins that will read input from the buttons
const byte buttonPin [] = {6, 7, 8, 9};

const short numberOfOptions = sizeof(ledPin) / sizeof(byte);

// counter variable
int i;

// variable to hold which button is pressed
byte whichButton;

// constant to hold the max number of rounds that can be played
const byte maxRounds = 100;

// variable to count the value of the current round
byte roundCounter = 0;

// setup array to hold the series of prompts for every round
byte promptArray [maxRounds];

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
	Serial.print("Round number"); Serial.println(roundCounter);

	// generate a random number between 0 and 3 and put it in the next available spot in the prompt array
	promptArray[roundCounter] = random(4);

	// blink each led in the sequence of prompts
	for (i = 0; i <= roundCounter; i++) {
		digitalWrite(ledPin[promptArray[i]], HIGH);

		// blink length
		delay(200);

		digitalWrite(ledPin[promptArray[i]], LOW);

		// delay between led blinks
		delay(100);
	}

	Serial.println("Wait for correct button sequence.");

	// set play counter 0 for new round
	i = 0;

	// a loop to check the value of the button until it turns low
	// after a correct button press, the round counter will increment and the loop will wait for the next correct button press in the sequence
	while(1) {
		// check if a button is pressed and which one
		whichButton = checkButton();

		// check if a button has been pressed
		if (whichButton != 4) {
			// if a button has been pressed, check if it's the same as the prompt
			if (whichButton == promptArray[i]) {
				digitalWrite(ledPin[promptArray[i]], HIGH);

				// blink length
				delay(200);

				digitalWrite(ledPin[promptArray[i]], LOW);

				// wait for button to depress
				while (whichButton != 4) {
					whichButton = checkButton();
				}
			}

			// if the play counter is equal to the round counter, the sequence has been completed.
			// exit the user round loop
			if (i == roundCounter) {
				delay(1000);
				break;
			}

			// increment play counter so the while loop waits for the next prompt in the sequence
			i++;
		}

		// wait a little bit to prevent false readings due to bouncing
		delay(20);
	}

	Serial.println("Correct sequence pressed!");

	Serial.println("Incrementing roundCounter.");
	roundCounter++;

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
