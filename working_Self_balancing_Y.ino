#include "Wire.h"
#include <MPU6050_light.h>

const int IN1 = 4;
const int IN2 = 5;
const int IN3 = 6;
const int IN4 = 7;
const int ENA = 10;
const int ENB = 11;

float desiredAngle = 0.00;
float error = 0.00;
float errorSum = 0.00;
float prevError = 0.00;
float U = 0.00;
float kp = 30;
float ki = 0.0;
float kd = 6;
float initialAngle = 0;

MPU6050 mpu(Wire);

void setup() {
  Serial.begin(9600);
  Wire.begin();
  byte status = mpu.begin();
  Serial.print(F("MPU6050 status: "));
  Serial.println(status);
  while(status != 0){}
  Serial.println(F("Calculating offsets, do not move MPU6050"));
  delay(1000);
  mpu.calcOffsets();
  delay(100);
  Serial.println("Done!\n");
  initialAngle = mpu.getAngleX();
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
}

void move(int PwmL, int PwmR) {
  digitalWrite(IN1, PwmL > 0);
  digitalWrite(IN2, PwmL < 0);
  digitalWrite(IN3, PwmR < 0);
  digitalWrite(IN4, PwmR > 0);
  analogWrite(ENA, abs(PwmL));
  analogWrite(ENB, abs(PwmR));
}

float getAngle() {
  mpu.update();
  return mpu.getAngleX() - initialAngle;
}

void loop() {
  float angle = getAngle();

  if (abs(angle) > 45) {
    move(0, 0);
    errorSum = 0;
    prevError = 0;
    error = 0;
    delay(10);
    return;
  }

  prevError = error;
  error = desiredAngle - angle;
  errorSum += error;
  errorSum = constrain(errorSum, -300, 300);
  U = kp * error + ki * errorSum + kd * (error - prevError);
  U = constrain(U, -255, 255);

  move(-U, -U);

  Serial.print("Angle.: ");
  Serial.print(angle);
  Serial.print("\tError: ");
  Serial.print(error);
  Serial.print("\tU: ");
  Serial.println(U);

  delay(10);
}
