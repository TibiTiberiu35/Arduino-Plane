#include <Wire.h>
#include <Servo.h>

Servo servoCarma;
Servo servoFlapsuri;
Servo escMotor;

long accelX, accelY, accelZ;
float fortaGX, fortaGY, fortaGZ;

long gyroX, gyroY, gyroZ;
float rotX, rotY, rotZ;

float limita = 0.10;
long distantaParcursa = 0;

void setup() {
  Serial.begin(9600);
  Wire.begin();
  setupMPU();
  servoCarma.attach(8);
  servoFlapsuri.attach(9);
  escMotor.attach(12);
  delay(3000);
}


void loop() {
  rotateMotor();
  inregistreazaRegistreAccel();
  recordGyroRegisters();
  calculeazaDistantaParcursa();
  intoarcere();
  rotireFlapsuri();
  printData();
  delay(100);
}

void setupMPU(){
  Wire.beginTransmission(0b1101000); //This is the I2C address of the MPU (b1101000/b1101001 for AC0 low/high datasheet sec. 9.2)
  Wire.write(0x6B); //Accessing the register 6B - Power Management (Sec. 4.28)
  Wire.write(0b00000000); //Setting SLEEP register to 0. (Required; see Note on p. 9)
  Wire.endTransmission();  
  Wire.beginTransmission(0b1101000); //I2C address of the MPU
  Wire.write(0x1B); //Accessing the register 1B - Gyroscope Configuration (Sec. 4.4) 
  Wire.write(0x00000000); //Setting the gyro to full scale +/- 250deg./s 
  Wire.endTransmission(); 
  Wire.beginTransmission(0b1101000); //I2C address of the MPU
  Wire.write(0x1C); //Accessing the register 1C - Acccelerometer Configuration (Sec. 4.5) 
  Wire.write(0b00001000); //Setting the accel to +/- 2g
  Wire.endTransmission(); 
}

void inregistreazaRegistreAccel() {
  Wire.beginTransmission(0b1101000); //I2C address of the MPU
  Wire.write(0x3B); //Starting register for Accel Readings
  Wire.endTransmission();
  Wire.requestFrom(0b1101000,6); //Request Accel Registers (3B - 40)
  while(Wire.available() < 6);
  accelX = Wire.read()<<8|Wire.read(); //Store first two bytes into accelX
  accelY = Wire.read()<<8|Wire.read(); //Store middle two bytes into accelY
  accelZ = Wire.read()<<8|Wire.read(); //Store last two bytes into accelZ
  proceseazaDateAccel();
}

void proceseazaDateAccel(){
  fortaGX = accelX / 16384.0;
  fortaGY = accelY / 16384.0; 
  fortaGZ = accelZ / 16384.0;
}

void recordGyroRegisters() {
  Wire.beginTransmission(0b1101000); //I2C address of the MPU
  Wire.write(0x43); //Starting register for Gyro Readings
  Wire.endTransmission();
  Wire.requestFrom(0b1101000,6); //Request Gyro Registers (43 - 48)
  while(Wire.available() < 6);
  gyroX = Wire.read()<<8|Wire.read(); //Store first two bytes into accelX
  gyroY = Wire.read()<<8|Wire.read(); //Store middle two bytes into accelY
  gyroZ = Wire.read()<<8|Wire.read(); //Store last two bytes into accelZ
  proceseazaDateGyro();
}

void proceseazaDateGyro() {
  rotX = gyroX / 131.0;
  rotY = gyroY / 131.0; 
  rotZ = gyroZ / 131.0;
}

void printData() {
  Serial.print("Gyro (deg)");
  Serial.print(" X=");
  Serial.print(rotX);
  Serial.print(" Y=");
  Serial.print(rotY);
  Serial.print(" Z=");
  Serial.print(rotZ);
  Serial.print(" Accel (g)");
  Serial.print(" X=");
  Serial.print(fortaGX);
  Serial.print(" Y=");
  Serial.print(fortaGY);
  Serial.print(" Z=");
  Serial.print(fortaGZ);
  Serial.print("Distanta parcursa: ");
  Serial.println(distantaParcursa);
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

void calculeazaDistantaParcursa() {
  if(rotZ > 2) {
    distantaParcursa += rotZ;
  }
}

void intoarcere() {
  if(distantaParcursa > 1000) {
    distantaParcursa = 0;
    miscaCarmaSpreDreapta();
  }
}

void rotireFlapsuri() {
  if(limita < fortaGZ) {
    servoFlapsuri.write(45);
  } else if(-limita > fortaGZ) {
    servoFlapsuri.write(135);
  } else {
    servoFlapsuri.write(90);
  }
}
