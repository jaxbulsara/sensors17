// include library for stepper motor
#include <Stepper.h>

// important note from library
// The sequence of control signals for 4 control wires is as follows:
// In other words, looping through these 4 signals will result in continuous rotation
// This is handled by the Stepper object
//
// Step C0 C1 C2 C3
// 1  1  0  1  0
// 2  0  1  1  0
// 3  0  1  0  1
// 4  1  0  0  1

/*-----( Declare Constants, Pin Numbers )-----*/
// Number of steps per revolution of INTERNAL motor in 4-step mode
#define STEPS_PER_MOTOR_REVOLUTION 32

// Steps per OUTPUT SHAFT of gear reduction
#define STEPS_PER_OUTPUT_REVOLUTION 32 * 64  //2048

#define CW_BUTTON 7
#define CCW_BUTTON 6

/*-----( Declare objects )-----*/
// create an instance of the stepper class, specifying the number of steps of the motor and the pins it's attached to

// The pin connections need to be pins 11, 10, 9, 8 connected to Motor Driver In1, In2, In3, In4

// Then the pins are entered here in the sequence 1-3-2-4 for proper sequencing
// This will setup the motor pins as outputs
Stepper small_stepper(STEPS_PER_MOTOR_REVOLUTION, 11, 9, 10, 8);

/*-----( Declare Variables )-----*/
// holds the number of steps to take
// if positive - motor moves CW
// if negative - motor moves CCW
int steps;

// holds the speed of the motor in rpm
int speed;

void setup() {
	Serial.begin(9600);

	// setup CW rotation button
	pinMode(CW_BUTTON, INPUT_PULLUP);

	// setup CCW rotation button
	pinMode(CCW_BUTTON, INPUT_PULLUP);

	// set motor speed
	small_stepper.setSpeed(640);
}

void loop() {
	// while CW button is held down, turn motor clockwise
	while (digitalRead(CW_BUTTON) == 0) {
		small_stepper.step(1);
	}

	// while CCW button is held down, turn motor counterclockwise
	while (digitalRead(CCW_BUTTON) == 0) {
		small_stepper.step(-1);
	}
}
