#define THERMISTOR_PIN A0

// resistance of the resistor placed in series with the thermistor
int series_resistor = 10000;

// create a positive integer variable to hold the analog reading. initialze with value of 0 for average calculation loop
float reading = 0;

// create a floating point value to hold the thermistor resistance value
float thermistor_resistance;

// get an average reading over a given number of samples to reduce noise
byte mean_sample_num = 10;

void setup(void) {
	// initialize serial output at frequency of 9600 baud, or bits per second
	Serial.begin(9600);
}

void loop(void) {
	// start i at 0 and increment by 1 while it is less than the number of samples to take
	for (byte i = 0; i < mean_sample_num; i++) {
		// add analog readings to reading for given number of samples
		reading += analogRead(THERMISTOR_PIN);
	}

	// calculate mean value
	reading = reading / mean_sample_num;

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
