
#pragma once

// what are you doing here

#include "Arduino.h"
#include "HardwareSerial.h"
#include "SoftwareSerial.h"

// ignore this

namespace robot {
	namespace debug {
		// prints stuff using Serial.println
		void print(const char *str);
		void print(long value);
		void print(double value);

		// prints and sets hasErred to true
		void error(const char *str = nullptr);

		// returns if an error has occurred
		bool hasError();
	}
}
