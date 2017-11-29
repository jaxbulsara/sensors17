#include "SR04.h"
#define TRIG_PIN 12
#define ECHO_PIN 11
#define BLUE 3
#define GREEN 5
#define RED 6
#define delayTime 250// fading time between colors
SR04 sr04 = SR04(ECHO_PIN,TRIG_PIN);
long a;
int redValue;
int greenValue;
int blueValue;

void setup() {
   Serial.begin(9600);
   delay(250);
   pinMode(RED, OUTPUT);
pinMode(GREEN, OUTPUT);
pinMode(BLUE, OUTPUT);
digitalWrite(RED,LOW);
digitalWrite(GREEN, LOW);
digitalWrite(BLUE, LOW);
}


void loop() {
   a=sr04.Distance();
   Serial.print(a);
   Serial.println("cm");
   if ((1 < a) & (a < 25)) {
  
redValue = 255; // choose a value between 1 and 255 to change the color.
greenValue = 0;
blueValue = 0;

// this is unnecessary as we've either turned on RED in SETUP
// or in the previous loop ... regardless, this turns RED off
// analogWrite(RED, 0);
// delay(1000);

for(int i = 0; i < 255; i += 20) // fades out red bring green full when i=255
{
redValue -= 1;
greenValue += 1;
// The following was reversed, counting in the wrong directions
// analogWrite(RED, 255 - redValue);
// analogWrite(GREEN, 255 - greenValue);
analogWrite(RED, redValue);
analogWrite(GREEN, greenValue);
delay(5);
}

redValue = 0;
greenValue = 255;
blueValue = 0;

for(int i = 0; i < 255; i += 20) // fades out green bring blue full when i=255
{
greenValue -= 1;
blueValue += 1;
// The following was reversed, counting in the wrong directions
// analogWrite(GREEN, 255 - greenValue);
// analogWrite(BLUE, 255 - blueValue);
analogWrite(GREEN, greenValue);
analogWrite(BLUE, blueValue);
delay(5);
}

redValue = 0;
greenValue = 0;
blueValue = 255;

for(int i = 0; i < 255; i += 10) // fades out blue bring red full when i=255
{
// The following code has been rearranged to match the other two similar sections
blueValue -= 1;
redValue += 1;
// The following was reversed, counting in the wrong directions
// analogWrite(BLUE, 255 - blueValue);
// analogWrite(RED, 255 - redValue);
analogWrite(BLUE, blueValue);
analogWrite(RED, redValue);
delay(5);
}
digitalWrite(RED,LOW);
digitalWrite(GREEN, LOW);
digitalWrite(BLUE, LOW);   }
   delay(1);
}
