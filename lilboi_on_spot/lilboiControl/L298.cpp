
#include "L298.h"

// see L298.h for function descriptions

L298::L298() {
	// do nothing
}

void L298::setup() {
	if (!isLeftSetup) {
		robot::debug::error("Pins of left motor of L298 not assigned, use setLeftMotorPins(...)");
		return;
	}

	if (!isRightSetup) {
		robot::debug::error("Pins of right motor of L298 not assigned, use setRightMotorPins(...)");
		return;	
	}

	pinMode(ENA, OUTPUT);
	pinMode(IN1, OUTPUT);
	pinMode(IN2, OUTPUT);
	pinMode(ENB, OUTPUT);
	pinMode(IN3, OUTPUT);
	pinMode(IN4, OUTPUT);

	digitalWrite(IN1, HIGH ^ leftDirection);
	digitalWrite(IN2, LOW ^ leftDirection);

	digitalWrite(IN3, HIGH ^ rightDirection);
	digitalWrite(IN4, LOW ^ rightDirection);

	isSetup = true;
}

void L298::setLeftMotorPins(int ENA, int IN1, int IN2) {
	if (isLeftSetup) {
		robot::debug::error("Cannot call setLeftMotorPins(int, int, int) twice");
		return;
	}

	this->ENA = ENA;
	this->IN1 = IN1;
	this->IN2 = IN2;

	isLeftSetup = true;
}

void L298::setRightMotorPins(int ENB, int IN3, int IN4) {
	if (isRightSetup) {
		robot::debug::error("Cannot call setRightMotorPins(int, int, int) twice");
		return;
	}

	this->ENB = ENB;
	this->IN3 = IN3;
	this->IN4 = IN4;

	isRightSetup = true;
}

void L298::setLeftMotorSpeed(int speed) {
	if (!isSetup) {
		robot::debug::error("Cannot call setLeftMotorSpeed(int) until setup() has been called");
		return;
	}

	digitalWrite(IN1, (speed > 0 ? HIGH : LOW) ^ leftDirection);
	digitalWrite(IN2, (speed > 0 ? LOW : HIGH) ^ leftDirection);
	analogWrite(ENA, speed < 0 ? -speed : speed);
}

void L298::setRightMotorSpeed(int speed) {
	if (!isSetup) {
		robot::debug::error("Cannot call setRightMotorSpeed(int) until setup() has been called");
		return;
	}

	digitalWrite(IN3, (speed > 0 ? HIGH : LOW) ^ rightDirection);
	digitalWrite(IN4, (speed > 0 ? LOW : HIGH) ^ rightDirection);
	analogWrite(ENB, speed < 0 ? -speed : speed);
}

void L298::setMotorSpeed(int speed) {
	if (!isSetup) {
		robot::debug::error("Cannot call setMotorSpeed(int) until setup() has been called");
		return;
	}

	setRightMotorSpeed(speed);
	setLeftMotorSpeed(speed);
}

void L298::stop() {
	if (!isSetup) {
		robot::debug::error("Cannot call stop() until setup() has been called");
		return;
	}

	setRightMotorSpeed(0);
	setLeftMotorSpeed(0);
}

void L298::reverseLeftMotorDirection() {
	leftDirection ^= HIGH;
}

void L298::reverseRightMotorDirection() {
	rightDirection ^= HIGH;
}
