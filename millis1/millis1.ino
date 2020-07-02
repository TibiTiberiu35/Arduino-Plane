#include <Servo.h>

unsigned long startMillis;  //some global variables available anywhere in the program
unsigned long currentMillis;
unsigned long millisUntilNow;
const byte ledPin = 13;    //using the built in LED
Servo escMotor;

void setup()
{
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  escMotor.attach(12);
  escMotor.write(120);
  startMillis = millis();
}

void loop()
{
  currentMillis = millis();  //get the current "time" (actually the number of milliseconds since the program started)
  millisUntilNow = currentMillis -startMillis;
  Serial.println(millisUntilNow);
  if(millisUntilNow >= 4000 && millisUntilNow < 7000) {
    escMotor.write(170);
  } else if(millisUntilNow >= 7000 && millisUntilNow < 9000) { 
    escMotor.write(110);  
  } else if(millisUntilNow >= 9000) { 
      Serial.print("Detached ");
      escMotor.detach(); 
  }
}
