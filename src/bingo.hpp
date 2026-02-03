#ifndef BINGO_HPP
#define BINGO_HPP

#pragma once

#include <Arduino.h>


// Typen
typedef enum processState_t { DONE, INIT, RUNNING } processState_t;

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

#endif	// BINGO_HPP

