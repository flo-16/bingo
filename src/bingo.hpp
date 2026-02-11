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
} Config_t;	// Konfigurationsstruktur

class Button {
	private:
		const uint8_t pin;
	public:
		Button(Config_t &rg) : pin(rg.btnPin) { pinMode(pin, INPUT_PULLUP); }
		void update(Config_t &rg) { if(digitalRead(pin) == LOW) rg.bupressed++; }
};

class Manager {
	private:
		uint8_t max;
	public:
		Manager(Config_t &rg) : max(rg.prMax) {}
		void update(Config_t &rg);
};

class Show {
	private:
		const gpios_t leds; 
		uint8_t dat;
	public:
		Show(Config_t &rg):leds(rg.leds), dat(0) {}
		void init();
		void update(const Config_t &rg);
};

class JobBasis {
	public:
		const uint8_t ID;		
		uint8_t &out;
		processState_t state;
		JobBasis(const uint8_t prID, uint8_t &rOut) : ID(prID), out(rOut), state(NONE) {}
		virtual void doInit() const = 0;
		virtual void doRun() const = 0;
	  void update(const uint8_t prID);
};

class  JobNext : public JobBasis {
	public:
		uint16_t holdTime;
		uint8_t initVal;
		JobNext(const uint8_t prID, Config_t &rg, const uint16_t prHoldTime, const uint8_t prInitVal) :
			JobBasis(prID, rg.output), holdTime(prHoldTime), initVal(prInitVal) {}
		virtual void doInit() const override { out = initVal; }
		virtual void doRun() const override { out = 0x00; }
};


// Implementationen

void Manager::update(Config_t &rg) {
 	if(rg.bupressed > 0) {
		uint8_t temp = rg.id + 1;
		rg.id = temp % (max + 1);
		rg.bupressed = 0;
		rg.buttonFlag = 1;
	};
	if(rg.id == 0) rg.output = 0;
}

void Show::init() {
	for(uint8_t i = 0; i < 8; i++) {
		pinMode(leds.pin[i], OUTPUT);
		digitalWrite(leds.pin[i], LOW);
	}
}

void Show::update(const Config_t &rg) {
	if(dat != rg.output) return;
	dat = rg.output;
	for(uint8_t i = 0; i < 8; i++) {
		digitalWrite(leds.pin[i], (dat >> i) & 0x01);
	}
}

void JobBasis::update(const uint8_t prID) {
	if(prID != ID) { state = NONE; return; }
	if(prID == ID && state == NONE) { state = SLEEP; out = 0; return; }
	if(state == SLEEP) { state = INIT; doInit(); return; }
	if(state == INIT) { state = RUNNING; doRun(); return; }
	if(state == RUNNING) { doRun(); return; }		
}

#endif	// BINGO_HPP

