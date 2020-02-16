#include <Servo.h>
#include <TimeAlarms.h>

Servo escMotor;
Servo servoCarma;

void setup() {
  Serial.begin(9600);
  servoCarma.attach(13); // stanga merge drept dreapta merge intr-o parte
  escMotor.attach(12);
  delay(3000);
}

void loop() {
  rotateMotor();
  delay(5000);
  miscaCarmaSpreDreapta();
}

void rotateMotor() {
  escMotor.write(111);
  delay(15);
}

void miscaCarmaSpreDreapta() {
  for (int i = 1; i <= 120; i++) {
    servoCarma.write(i);
    delay(10);
  }

  delay(3000);

  for (int i = 120; i >= 0; i--) {
    servoCarma.write(i);
  }
}
