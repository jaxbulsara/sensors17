#define THERMISTOR_PIN A0

// include liquid crystal (character display) library code:
#include <LiquidCrystal.h>

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// resistance of the resistor placed in series with the thermistor
int series_resistor = 10000;

// create a positive integer variable to hold the analog reading. initialze with value of 0 for average calculation loop
float reading = 0;

// create a floating point value to hold the thermistor resistance value
float thermistor_resistance;

// get an average reading over a given number of samples to reduce noise
byte mean_sample_num = 10;

// String to hold temperature for display
String tempStr = String();

void setup(void) {
	// initialize serial output at frequency of 9600 baud, or bits per second
	Serial.begin(9600);
  
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);

  // print out data headers
  lcd.print("Res:");

  lcd.setCursor(0,1);

  lcd.print("Temp:");
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

  lcd.setCursor(4, 0);
  lcd.print((int)thermistor_resistance);
  lcd.setCursor(10,0);
  lcd.print("Ohms");

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

	// set cursor to column 0 line 0 (first row)
	lcd.setCursor(5,1);

  // display temperature on display
  lcd.print(temperature, 1);

  lcd.setCursor(10,1);
  lcd.print("Deg F");

  // wait before next loop
  delay(500);
}
