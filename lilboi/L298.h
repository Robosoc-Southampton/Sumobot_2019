
#pragma once

#include "Arduino.h"
#include "robot.h"

// Wrapper class around the L298 motor controller
class L298 {
private:
	int ENA, ENB, IN1, IN2, IN3, IN4;
	bool isLeftSetup, isRightSetup, isSetup;
	int leftDirection, rightDirection;

public:
	// constructor does nothing
	L298();

	// initially sets up the motor controller interface, call set*MotorPins() first
	void setup();

	// sets the pins to be used to control the left and right motors
	// make sure ENA and ENB are both PWM pins
	void setLeftMotorPins(int ENA, int IN1, int IN2);
	void setRightMotorPins(int ENB, int IN3, int IN4);

	// sets the speed of the motors
	// use an integer `-255 <= speed <= 255`
	void setLeftMotorSpeed(int speed);
	void setRightMotorSpeed(int speed);
	void setMotorSpeed(int speed);

	// stops both motors
	void stop();

	// toggles the direction of the motors
	// should be called once in arduino's setup() based on the wiring of the robot
	void reverseLeftMotorDirection();
	void reverseRightMotorDirection();
};
