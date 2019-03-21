
#include "LED.h"

// see LED.h for function descriptions

LED::LED() {
	// do nothing
}

void LED::attach(int pin) {
	if (isSetup) {
		robot::debug::error("Cannot call attach(int) after setup() has been called");
		return;
	}

	this->pin = pin;
}

void LED::setup() {
	if (pin == -1) {
		robot::debug::error("Cannot call setup() before attach(int) has been called");
		return;
	}

	pinMode(pin, OUTPUT);

	isSetup = true;
}

void LED::pwmOn(int value) {
	if (!isSetup) {
		robot::debug::error("Cannot call pwmOn(int) before setup() has been called");
		return;
	}

	analogWrite(pin, value);
	state = true;
}

void LED::on() {
	if (!isSetup) {
		robot::debug::error("Cannot call on() before setup() has been called");
		return;
	}

	digitalWrite(pin, HIGH);
	state = true;
}

void LED::off() {
	if (!isSetup) {
		robot::debug::error("Cannot call off() before setup() has been called");
		return;
	}

	digitalWrite(pin, LOW);
	state = false;
}

void LED::toggle() {
	if (!isSetup) {
		robot::debug::error("Cannot call toggle() before setup() has been called");
		return;
	}

	digitalWrite(pin, state ? LOW : HIGH);
	state = !state;
}
