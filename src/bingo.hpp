#ifndef BINGO_HPP
#define BINGO_HPP

#pragma once

#include <Arduino.h>

// Typen
typedef enum processState_t { NONE, SLEEP, INIT, RUNNING } processState_t;

// Klassen
class Bingo_Button {
	private:
		uint8_t pin;
	public:
		Bingo_Button(uint8_t p) : pin(p) {
				pinMode(pin, INPUT_PULLUP);
		}
		void update(uint8_t &state) {
				if(digitalRead(pin) == LOW) state++;
		}
};

class Bingo_ProcessManager {
	private:
		uint8_t max;
	public:
		Bingo_ProcessManager(uint8_t m) : max(m) {}
		void update(uint8_t &id, uint8_t state) {
			if(state > 0) {
				id++;
				if(id > max) id = 1;
		};
	}
};

#endif	// BINGO_HPP

