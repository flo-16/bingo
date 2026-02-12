#ifndef BINGO_HPP
#define BINGO_HPP

#pragma once

#include <Arduino.h>

// Typen
typedef enum { NONE = 0, SLEEP, INIT, RUNNING } prState_t;													// möglichr Prozess-Zustände
typedef struct { uint8_t pin[8]; } ledPins_t;																				// nimmt LED-GPIOs auf
typedef struct {
	const uint8_t prMax;
	const ledPins_t leds;
	const uint8_t btnPin;
	uint8_t id;
	uint8_t output;
} Config_t;	// Konfigurationsstruktur

class Button {
	private:
		const uint8_t pin;
		const uint8_t prMax;		
	public:
		Button(Config_t &rg) : pin(rg.btnPin), prMax(rg.prMax) { pinMode(pin, INPUT_PULLUP); }
		void update(Config_t &rg);
};

class Show {
	private:
		const ledPins_t leds; 
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
		mutable prState_t state;
		mutable uint32_t timeStamp;
		JobBasis(const uint8_t prID, uint8_t &rOut) : ID(prID), out(rOut), state(NONE) {}
		virtual void doInit() const = 0;
		virtual void doRun() const = 0;
	  void update(const uint8_t prID);
};

class  JobNext : public JobBasis {
	public:
		const uint16_t holdTime;
		const uint8_t initVal;
		JobNext(const uint8_t prID, Config_t &rg, const uint16_t prHoldTime, const uint8_t prInitVal) :
			JobBasis(prID, rg.output), holdTime(prHoldTime), initVal(prInitVal) {}
		virtual void doInit() const override;
		virtual void doRun() const override;
};

class  JobPrev : public JobBasis {
	public:
		const uint16_t holdTime;
		const uint8_t initVal;
		JobPrev(const uint8_t prID, Config_t &rg, const uint16_t prHoldTime, const uint8_t prInitVal) :
			JobBasis(prID, rg.output), holdTime(prHoldTime), initVal(prInitVal) {}
		virtual void doInit() const override;
		virtual void doRun() const override;
};

// Implementationen

void Button::update(Config_t &rg) {
	static uint32_t lastTime = 0;
	uint32_t now = millis();
	if((now > lastTime) && (digitalRead(pin) == LOW)) {
		lastTime = now + 200;  // Entprellzeit
		rg.id++;
		rg.id %= prMax;
		rg.output = 0;
	}
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
	if(prID == ID && state == NONE) { state = SLEEP; return; }
	if(state == SLEEP) { state = INIT; doInit(); return; }
	if(state == INIT) { state = RUNNING; doRun(); return; }
	if(state == RUNNING) { doRun(); return; }		
}

void JobNext::doInit() const {
	out = initVal;
	timeStamp = millis() + holdTime;
}

void JobNext::doRun() const {
	if(millis() >= timeStamp) {
		out <<= 1;
		if(out == 0) out = initVal;
		timeStamp += holdTime;
	}
}

void JobPrev::doInit() const {
	out = initVal;
	timeStamp = millis() + holdTime;
	state = RUNNING;
}

void JobPrev::doRun() const {
	if(millis() >= timeStamp) {
		out >>= 1;
		if(out == 0) out = initVal;
		timeStamp += holdTime;
	}
}

#endif	// BINGO_HPP

