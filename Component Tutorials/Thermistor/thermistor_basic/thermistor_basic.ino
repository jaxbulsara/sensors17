#define THERMISTOR_PIN A0

// resistance of the resistor placed in series with the thermistor
int series_resistor = 10000;

// create a floating point variable to hold the analog reading
float reading;

// create a floating point variable to hold the thermistor resistance
float thermistor_resistance;

void setup(void) {
	// initialize serial output at frequency of 9600 baud, or bits per second
	Serial.begin(9600);
}

void loop(void) {
	// take a voltage reading of the analog pin and put it in reading
	reading = analogRead(THERMISTOR_PIN);

	// print reading value to serial monitor
	Serial.print("Analog reading: ");
	Serial.println(reading);

	// use the reading to calculate the thermistor resistance
	thermistor_resistance = series_resistor * reading / (1023.0 - reading);

	// print resistor value to serial monitor
	Serial.print("thermistor resistance: ");
	Serial.println(thermistor_resistance);

	// calculate temperature from resistance
	// thermistor used in this code is a MF52D-103f-3950
	int B = 3950;
	float T0 = 298.15;

	float temperature;
	temperature = thermistor_resistance / series_resistor;	// (R/Ro)
	temperature = log(temperature);				// ln(R/Ro)
	temperature /= B;							// 1/B * ln(R/Ro)
	temperature += 1.0 / T0;					// + (1/To)
	temperature = 1.0 / temperature;			// Invert
	temperature -= 273.15;						// convert to C
	temperature = temperature * 9 / 5 + 32;		// convert to F

	// print temperature to serial monitor
	Serial.print("Temperature reading: ");
	Serial.print(temperature);
	Serial.println(" *F");

	// wait 1 second before restarting the loop
	delay(1000);

	// print a newline to separate the next loop's output
	Serial.println();
}
