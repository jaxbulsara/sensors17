/*
   BYJ48 Stepper motor code
   Connect :
   IN1 >> D8
   IN2 >> D9
   IN3 >> D10
   IN4 >> D11
   VCC ... 5V Prefer to use external 5V Source
   Gnd
   written By :Mohannad Rawashdeh
  https://www.instructables.com/member/Mohannad+Rawashdeh/
     28/9/2013
  */

// define pin numbers for stepper motor
#define IN1  8
#define IN2  9
#define IN3  10
#define IN4  11

int steps = 0;
boolean direction = true;// gre
unsigned long last_time;
unsigned long currentMillis ;
int steps_left=4095;
long time;

void setup() {
	Serial.begin(115200);

	// setup pins as outputs
	pinMode(IN1, OUTPUT);
	pinMode(IN2, OUTPUT);
	pinMode(IN3, OUTPUT);
	pinMode(IN4, OUTPUT);
	// delay(1000);
}
void loop()
{
  while(steps_left>0){
  currentMillis = micros();
  if(currentMillis-last_time>=1000){
  stepper(1);
  time=time+micros()-last_time;
  last_time=micros();
  steps_left--;
  }
  }
   Serial.println(time);
  Serial.println("Wait...!");
  delay(2000);
  direction=!direction;
  steps_left=4095;
}

void stepper(int xw){
  for (int x=0;x<xw;x++){
switch(steps){
   case 0:
     digitalWrite(IN1, LOW);
     digitalWrite(IN2, LOW);
     digitalWrite(IN3, LOW);
     digitalWrite(IN4, HIGH);
   break;
   case 1:
     digitalWrite(IN1, LOW);
     digitalWrite(IN2, LOW);
     digitalWrite(IN3, HIGH);
     digitalWrite(IN4, HIGH);
   break;
   case 2:
     digitalWrite(IN1, LOW);
     digitalWrite(IN2, LOW);
     digitalWrite(IN3, HIGH);
     digitalWrite(IN4, LOW);
   break;
   case 3:
     digitalWrite(IN1, LOW);
     digitalWrite(IN2, HIGH);
     digitalWrite(IN3, HIGH);
     digitalWrite(IN4, LOW);
   break;
   case 4:
     digitalWrite(IN1, LOW);
     digitalWrite(IN2, HIGH);
     digitalWrite(IN3, LOW);
     digitalWrite(IN4, LOW);
   break;
   case 5:
     digitalWrite(IN1, HIGH);
     digitalWrite(IN2, HIGH);
     digitalWrite(IN3, LOW);
     digitalWrite(IN4, LOW);
   break;
     case 6:
     digitalWrite(IN1, HIGH);
     digitalWrite(IN2, LOW);
     digitalWrite(IN3, LOW);
     digitalWrite(IN4, LOW);
   break;
   case 7:
     digitalWrite(IN1, HIGH);
     digitalWrite(IN2, LOW);
     digitalWrite(IN3, LOW);
     digitalWrite(IN4, HIGH);
   break;
   default:
     digitalWrite(IN1, LOW);
     digitalWrite(IN2, LOW);
     digitalWrite(IN3, LOW);
     digitalWrite(IN4, LOW);
   break;
}
Setdirection();
}
}
void Setdirection(){
if(direction==1){ steps++;}
if(direction==0){ steps--; }
if(steps>7){steps=0;}
if(steps<0){steps=7; }
}
