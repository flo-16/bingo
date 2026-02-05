#ifndef BINGO_HPP
#define BINGO_HPP

#pragma once

#include <Arduino.h>

// Typen
typedef enum processState_t { NONE = 0, SLEEP, INIT, RUNNING } processState_t;

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

class Bingo_Job_Basis {
	private:
		processState_t state;
		enum waitTime { ID_CHANGE = 200	};
	protected:
		uint8_t ID;			
		Bingo_Job_Basis(uint8_t prID) : ID(prID), state(NONE) {}
		virtual void doSleep() = 0;
		virtual void doInit() = 0;
		virtual void doRun() = 0;
	public:
	  void update(uint8_t prID) {
			if(prID != ID) { state = NONE; return; }
			if(prID == ID && state == NONE) { state = SLEEP; doSleep();delay(ID_CHANGE); return; }
			if(state == SLEEP) { state = INIT; doInit(); delay(ID_CHANGE); return; }
			if(state == INIT) { state = RUNNING; doRun(); delay(ID_CHANGE); return; }
			if(state == RUNNING) { doRun(); return; }		
		}
};

#endif	// BINGO_HPP

