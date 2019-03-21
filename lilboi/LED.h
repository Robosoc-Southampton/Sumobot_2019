
#pragma once

#include "robot.h"
#include "Arduino.h"

// wrapper around LEDs
class LED {
private:
	int pin = -1;
	bool isSetup, state;

public:
	LED();

	// set the pin to use to control the LED
	void attach(int pin);
	// sets up the LED
	void setup();

	// turns the LED on partially on (not fully bright)
	// note that this needs the LED to be plugged in to a PWM pin (LED_BUILTIN is not)
	// you can emulate PWM in your code, ask about it!
	void pwmOn(int value);

	// turns the LED on
	void on();

	// you guessed it, turns the LED off
	void off();

	// turns an on LED to off and an off LED to on
	void toggle();
};
