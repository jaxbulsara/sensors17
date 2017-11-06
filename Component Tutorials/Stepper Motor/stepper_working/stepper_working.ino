/* YourDuino.com Example Software Sketch
   Small Stepper Motor and Driver V1.5 06/21/17
   http://www.yourduino.com/sunshop/index.php?l=product_detail&p=126
   Shows 4-step sequence, Then 1/2 turn and back different speeds
   terry@yourduino.com */

/*-----( Import needed libraries )-----*/
#include <Stepper.h>

// important notes from library
// The sequence of control signals for 4 control wires is as follows:
// In other words, looping through these 4 signals will result in continuous rotation
// The Stepper object keeps track of where it is in this sequence, so all you need to do is tell it how much to turn
//
// Step C0 C1 C2 C3
// 1  1  0  1  0
// 2  0  1  1  0
// 3  0  1  0  1
// 4  1  0  0  1

/*-----( Declare Constants, Pin Numbers )-----*/
//---( Number of steps per revolution of INTERNAL motor in 4-step mode )---
#define STEPS_PER_MOTOR_REVOLUTION 32

//---( Steps per OUTPUT SHAFT of gear reduction )---
#define STEPS_PER_OUTPUT_REVOLUTION 32 * 64  //2048

/*-----( Declare objects )-----*/
// create an instance of the stepper class, specifying the number of steps of the motor and the pins it's attached to

// The pin connections need to be pins 11, 10, 9, 8 connected to Motor Driver In1, In2, In3, In4

// Then the pins are entered here in the sequence 1-3-2-4 for proper sequencing
// This will setup the motor pins as outputs
Stepper small_stepper(STEPS_PER_MOTOR_REVOLUTION, 11, 9, 10, 8);


/*-----( Declare Variables )-----*/
int  Steps2Take;

void setup()   /*----( SETUP: RUNS ONCE )----*/
{
	Serial.begin(9600);
}/*--(end setup )---*/

void loop()   /*----( LOOP: RUNS CONSTANTLY )----*/
{
  // small_stepper.setSpeed(5);   // SLOWLY Show the 4 step sequence
  // Steps2Take  =  2;  // Rotate CW
  // small_stepper.step(Steps2Take);
  // delay(2000);

  // max speed to maintain accuracy is 700 rpm
  // the speed of the INTERNAL motor is set, not the output rpm

  Steps2Take  =  STEPS_PER_OUTPUT_REVOLUTION;  // Rotate CW 1 turn
  small_stepper.setSpeed(640);
  small_stepper.step(Steps2Take);
  delay(100);

  Steps2Take  =  - STEPS_PER_OUTPUT_REVOLUTION;  // Rotate CCW 1 turn
  small_stepper.setSpeed(640);
  small_stepper.step(Steps2Take);
  delay(100);

}/* --(end main loop )-- */

/* ( THE END ) */
