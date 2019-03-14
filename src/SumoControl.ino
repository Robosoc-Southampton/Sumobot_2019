#include "L298.h"

#define CH2_MID 1545
#define CH2_HALFRANGE 380L

byte PWM_PIN = 3;

int value = 0;
int32_t value_mid = 0;
int32_t speed = 0;

L298 motor;

void setup() {
  // put your setup code here, to run once:
  //PIN 3 is for comms, comms initialization
  pinMode(PWM_PIN, INPUT);
  Serial.begin(9600);

  //motors control initialization, change the pins maybe
  //void setLeftMotorPins(int ENA, int IN1, int IN2);
  motor.setLeftMotorPins(11, 10, 9);
  //void setRightMotorPins(int ENB, int IN3, int IN4);
  motor.setRightMotorPins(6, 5, 4);
  motor.setup();
}

void loop() {
  // put your main code here, to run repeatedly:
  value = pulseIn(PWM_PIN, HIGH);
  Serial.println(value);
  
  //asssumes channel 2 to PIN 3
  //rescaling of values
  value_mid = value - CH2_MID;
  Serial.println(value_mid);

  speed = -(value_mid*255) / CH2_HALFRANGE;

  
  Serial.println(speed);
  //limiting so its not out of range
  if(speed > 255)
  {
    speed = 255;
  }else if(speed < -255)
  {
    speed = -255;
  }
  motor.setLeftMotorSpeed(speed);
  delay(10);

}
