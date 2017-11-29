#include <Servo.h>; //includes the servo library, allows access to premade code
  //Button variables
  
  int buttonPin = 2; //sets digital pin 2 as our button pin
  
  boolean buttonState = LOW; //Boolean variable set to LOW
  
  boolean previousButtonState = LOW; //Boolean Variable set to LOW
  
  //Servo variables
   const int servoPin = 9;  //sets digitial pin 9 as our servo pin
   
   Servo myservo; //uses code from the servo library to name our servo “myservo”
   
   boolean motorEnabled = 0; //variable that will be used to operate servo
   
void setup() { //setup function, runs once when the arduino code is uploaded/turned on
  
}
  //Servo
    myservo.attach (servoPin); //attaches servo to the digital pin 9 designated earlier
  //Button
    pinMode(buttonPin, INPUT); //sets the buttonPin as an INPUT
}
void loop() { // the loop routine runs over and over 
  //Servo+button
 buttonState = digitalRead(buttonPin); //sets the variable to equal the state of the buttonPin, it will read HIGH or LOW.

  if(previousButtonState != buttonState && buttonState == HIGH){
  // if the previous button state does not equal the current button state and the button state is equal to HIGH then the motorEnabled variable is set to anything besides what it currently is

    motorEnabled = !motorEnabled; //flips the value. 1 to 0 or 0 to 1
    }
  if(motorEnabled == 1){ //if motorEnabled is 1 
    myservo.write(180); // the servo rotates to 180 degrees
    }
  else{ //if motorEnabled is not 1
   myservo.write(10); // the servo rotates to 10 degrees
    }
  previousButtonState = buttonState; 

