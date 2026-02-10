#ifndef BINGO_HPP
#define BINGO_HPP

#pragma once

#include <Arduino.h>

// Typen
typedef enum { NONE = 0, SLEEP, INIT, RUNNING } processState_t;										// möglichr Prozess-Zustände
typedef struct { uint8_t pin[8]; } gpios_t;																				// nimmt LED-GPIOs auf
typedef struct {
	const uint8_t prMax;
	const gpios_t leds;
	const uint8_t btnPin;
	uint8_t id;
	uint8_t output;
	uint8_t buttonFlag;
	uint8_t bupressed;
	processState_t prState;

//	uint16_t jobHoldTime; 
} Config_t;	// Konfigurationsstruktur

//typedef gpios_t* pGpios_t;

// Klassen
class Button {
	private:
		uint8_t pin;
	public:
		Button(const uint8_t p) : pin(p) { pinMode(pin, INPUT_PULLUP); }
		void update(uint8_t &state) { if(digitalRead(pin) == LOW) state++; }
};

class Manager {
	private:
		uint8_t max;
	public:
		Manager(const uint8_t m) : max(m) {}
		void update(uint8_t &id, uint8_t state);
};

class Job_Basis {
	private:
		processState_t state;
		enum waitTime { ID_CHANGE = 200	};
	public:
		const uint8_t ID;		
		uint8_t &out;
		uint16_t holdTime;
		Job_Basis(const uint8_t prID, uint8_t &rOut, const uint16_t prHoldTime) : ID(prID), out(rOut), holdTime(prHoldTime), state(NONE) {}
		virtual void doInit() const = 0;
		virtual void doRun() const = 0;
	  void update(const uint8_t prID);
};

class  Job_Next : public Job_Basis {
	public:
		Job_Next(const uint8_t prID, uint8_t &rOut, const uint16_t prHoldTime) : Job_Basis(prID, rOut, prHoldTime) {}
		virtual void doInit() const override { out = 0xFF; }
		virtual void doRun() const override { out = 0x00; }
};


class Show {
	private:
		const gpios_t &leds; 
		uint8_t dat;
	public:
		Show(const gpios_t &rLeds):leds(rLeds), dat(0) {}
		void init();
		void update(const uint8_t out);
};

// Implementationen

void Manager::update(uint8_t &id, uint8_t state) {
 	if(state > 0) {
		uint8_t temp = id + 1;
		id = temp % (max + 1);
	};
 }

void Job_Basis::update(const uint8_t prID) {
	if(prID != ID) { state = NONE; return; }
	if(prID == ID && state == NONE) { state = SLEEP; out = 0; delay(ID_CHANGE); return; }
	if(state == SLEEP) { state = INIT; doInit(); delay(ID_CHANGE); return; }
	if(state == INIT) { state = RUNNING; doRun(); delay(ID_CHANGE); return; }
	if(state == RUNNING) { doRun(); return; }		
}

void Show::init() {
	for(uint8_t i = 0; i < 8; i++) {
		pinMode(leds.pin[i], OUTPUT);
		digitalWrite(leds.pin[i], LOW);
	}
}

void Show::update(const uint8_t out) {
	if(dat != out) return;
	dat = out;
	for(uint8_t i = 0; i < 8; i++) {
		digitalWrite(leds.pin[i], (dat >> i) & 0x01);
	}
}

#endif	// BINGO_HPP

