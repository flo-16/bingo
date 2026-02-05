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
		Bingo_Button(const uint8_t p) : pin(p) { pinMode(pin, INPUT_PULLUP); }
		void update(uint8_t &state) { if(digitalRead(pin) == LOW) state++; }
};

class Bingo_ProcessManager {
	private:
		uint8_t max;
	public:
		Bingo_ProcessManager(const uint8_t m) : max(m) {}
		void update(uint8_t &id, uint8_t state);
};

class Bingo_Job_Basis {
	private:
		processState_t state;
		enum waitTime { ID_CHANGE = 200	};
	protected:
		uint8_t ID;			
		Bingo_Job_Basis(const uint8_t prID) : ID(prID), state(NONE) {}
		virtual void doSleep() const = 0;
		virtual void doInit() const = 0;
		virtual void doRun() const = 0;
	public:
	  void update(const uint8_t prID);
};

// Implementationen

void Bingo_ProcessManager::update(uint8_t &id, uint8_t state) {
	if(state > 0) {
		id++;
		if(id > max) id = 1;
	};
}

void Bingo_Job_Basis::update(const uint8_t prID) {
	if(prID != ID) { state = NONE; return; }
	if(prID == ID && state == NONE) { state = SLEEP; doSleep(); delay(ID_CHANGE); return; }
	if(state == SLEEP) { state = INIT; doInit(); delay(ID_CHANGE); return; }
	if(state == INIT) { state = RUNNING; doRun(); delay(ID_CHANGE); return; }
	if(state == RUNNING) { doRun(); return; }		
}

#endif	// BINGO_HPP

