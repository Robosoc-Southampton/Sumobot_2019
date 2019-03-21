
#include "robot.h"

namespace robot {
	namespace debug {
		bool hasErred = true;

		void print(const char *str) {
			Serial.println(str);
		}

		void print(long value) {
			Serial.println(value);
		}

		void print(double value) {
			Serial.println(value);
		}

		void error(const char *str) {
			if (str != nullptr) {
				Serial.println(str);
			}

			robot::debug::hasErred = true;
		}

		bool hasError() {
			return robot::debug::hasErred;
		}
	}
}
