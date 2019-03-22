
#include "L298.h"
#include <Servo.h>

//Motors PINS:
byte ENA = 13;
byte IN1 = 12;
byte IN2 = 8;

byte IN3 = 7;
byte IN4 = 4;
byte ENB = 2;

//Motors:
L298 motor;

void setup() {
    Serial.begin(9600);

    motor.setLeftMotorPins(ENA, IN1, IN2);
    motor.setRightMotorPins(ENB, IN3, IN4);
    motor.setup();

    motor.setLeftMotorSpeed(128);
    motor.setRightMotorSpeed(-128);
}

void loop() {
}
