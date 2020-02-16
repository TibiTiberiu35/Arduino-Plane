#include <Wire.h>
#include <Servo.h>

Servo servo1;
Servo servo2;

long accelX, accelY, accelZ;
float fortaGX, fortaGY, fortaGZ;
float limita = 0.10;

void setup() {
  Serial.begin(9600);
  Wire.begin();
  setupMPU();
  servo1.attach(9);
  servo2.attach(10);
}


void loop() {
  inregistreazaRegistreAccel();
  //  arataDate();
  rotireFlapsuri();
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

//void arataDate() {
//  Serial.print(" Accel (g)");
//  Serial.print(fortaGZ);
//  delay(100);
//}

void rotireFlapsuri() {
  if(limita < fortaGZ) {
    servo1.write(45);
    servo2.write(45);
  } else if(-limita > fortaGZ) {
    servo1.write(135);
    servo2.write(135);
  } else {
    servo1.write(90);
    servo2.write(90);
  }
}
