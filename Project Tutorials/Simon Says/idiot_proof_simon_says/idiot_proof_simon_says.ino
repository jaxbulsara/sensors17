// Define constants - these values don't change throughout the code
// the pin that will control the led
const byte ledPin = 3

// the pin that will read input from the button
const byte buttonPin = 2

// setup function - runs once
void setup() {
	// setup pin modes as input or output
	pinMode(ledPin, OUTPUT);

	// button pin is floating value. It relies on the pull up resistor for it's value
	// unpressed - HIGH
	// pressed - LOW
	pinMode(buttonPin, INPUT);

	// keep led off by default
	digitalWrite(ledPin, LOW);

	// start serial monitor for debugging
	Serial.begin(9600);

	// print a debug statement
	Serial.println("Setup completed successfully.");
}

// loop function - runs continuously after setup() is run
void loop() {
	Serial.println("Blink led.");

	digitalWrite(ledPin, HIGH);

	// keep led on for 300 milliseconds
	delay(300);

	digitalWrite(ledPin, LOW);

	Serial.println("Wait for button press.");

	// a loop to check the value of the button until it turns low
	while (digitalRead(buttonPin) == HIGH) {
		// continue will move back to the contitional statement in the while loop
		continue;
	}

	Serial.println("Button pressed!\n");
}
