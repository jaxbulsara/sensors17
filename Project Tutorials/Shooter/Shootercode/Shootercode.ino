  
   #include <Servo.h>; //includes the servo library, allows access to premade code

  //DCmotor variables
  int DCmotor = 3; //initliazes a variable for the DCmotor and links it to the arduinos digitial pin 3
  //Pot Variables
  int PotInput = A0; //initliazes a variable for the potentiometer and links it to the arduinos analog pin 0
  //Button variables
  int buttonPin = 2;
  boolean buttonState = LOW;
  boolean previousButtonState = LOW;
  //Servo variables
   const int servoPin = 9;
   Servo myservo;
   boolean motorEnabled = 0;

void setup() { //setup function, runs once when the arduino code is uploaded/turned on
  //Servo
    myservo.attach (servoPin);
  //Button
    pinMode(buttonPin, INPUT);
 //Dcmotor 
  Serial.begin(9600);   // initialize serial communication at 9600 bits per second:

   pinMode(DCmotor, OUTPUT); //sets the mode of the DCmotor pin to an output 

}


void loop() { // the loop routine runs over and over 
  //Servo+button
 buttonState = digitalRead(buttonPin);

  if(previousButtonState != buttonState && buttonState == HIGH){
    motorEnabled = !motorEnabled;
    }
  if(motorEnabled == 1){
    myservo.write(180);
    }
  else{
   myservo.write(10);
    }
  previousButtonState = buttonState;

 
 //DCmotor
  int sensorValue = analogRead(PotInput); 
  //we create a new variable called sensorValue, 
 //this variable will be set equal to the voltage that the potentiometer sends as analog input to the A0 pin
  
  int speed = sensorValue/4; //this variable will be equal to the voltage of the sensorValue divided by 4 
  
  Serial.println(motorEnabled); //This will print out the speed value to the serial monitor, allows the user to view the potentiometers effect on the value

  if (speed >= 50 && speed <= 255) //only takes values that cause the motor to rotate
    {
      analogWrite(DCmotor, speed); //sends the inputed speed to the motor
    }

}
