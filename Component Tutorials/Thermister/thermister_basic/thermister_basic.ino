#include <stdio.h>

#define SERIES_RESISTOR 10000
#define THERMISTER_PIN A0

void setup(void) {
	// initialize serial output at frequency of 9600 baud, or bits per second
	Serial.begin(9600);
}

void loop(void) {
	// create an integer variable to hold the analog reading
	int reading;

	// create a string (character array) to hold serial output
	char serial_output[100];

	// get reading from analog pin and put in reading variable
	reading = analogRead(THERMISTER_PIN);

	// format a string to contain the reading value and put the formatted string in serial_output
	sprintf(serial_output, "Analog Reading: %d", reading);

	// print formatted string to serial monitor
	Serial.println(serial_output);

	// use the reading to calculate the thermister resistance
	reading = (1023 / reading) - 1;
	reading = SERIES_RESISTOR / reading;

	// format a string to contain the thermister resistance, now stored in the reading variable
	sprintf(serial_output, "Thermister resistance: %d", reading;);

	// print formatted string te serial monitor
	Serial.println(serial_output);

	// wait 1 second before restarting the loop
	delay(1000);

	// print a newline to separate the next loop's output
	Serial.println();
}
