#include <Servo.h>

unsigned long startMillis;  //some global variables available anywhere in the program
unsigned long currentMillis;
const byte ledPin = 13;    //using the built in LED
Servo escMotor;


void setup()
{
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  escMotor.attach(12);
  escMotor.write(160);
  startMillis = millis();
}

void loop()
{
  currentMillis = millis();  //get the current "time" (actually the number of milliseconds since the program started)
  if (currentMillis - startMillis >= 3000)  //test whether the period has elapsed
  {
    digitalWrite(ledPin, !digitalRead(ledPin));  //if so, change the state of the LED.  Uses a neat trick to change the state
    // startMillis = currentMillis;  //IMPORTANT to save the start time of the current LED state.
  } else if(currentMillis - startMillis >= 4000) {
    escMotor.write(170);
  } else if(currentMillis - startMillis >= 7000) { 
    escMotor.write(110);  
  } else if(currentMillis - startMillis >= 9000) { 
    escMotor.detach();  
  }
}
