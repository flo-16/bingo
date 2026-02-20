#ifndef BINGO_HPP
#define BINGO_HPP

#pragma once

#include <Arduino.h>

// Globaler Konfigurationstyp
typedef struct {
	const uint8_t prMax;
	const uint8_t leds[8];
	const uint8_t btnPin;
	const uint16_t holdTime[3];
	const uint8_t pattern[3];
	uint8_t id;
	uint8_t output;
} Config_t;	

class Button {
	private:
		Config_t &rg;
	public:
		Button(Config_t &rg) : rg(rg) { pinMode(rg.btnPin, INPUT_PULLUP); }
		void update();
};

class Handler {
	private:
		Config_t &rg;
		uint32_t nextTime;
	public:
		Handler(Config_t &rg) : rg(rg), nextTime(0) {}
		void update();		
};

class Show {
	private:
		Config_t &rg;
		uint8_t dat;
	public:
		Show(Config_t &rg) : rg(rg), dat(0) {
			for(uint8_t i = 0; i < 8; i++) {
				pinMode(rg.leds[i], OUTPUT);
				digitalWrite(rg.leds[i], LOW);
			}
		}
		void update();
};

// Implementierung der Methoden

void Button::update() {
	static uint32_t lastTime = 0;
	uint32_t now = millis();
	if((now > lastTime) && (digitalRead(rg.btnPin) == LOW)) {
		lastTime = now + 200;  // Entprellzeit
		rg.id++;
		rg.id %= rg.prMax;
	}
}	

void Handler::update() {
	uint32_t now = millis();
	if(now < nextTime) return;
	nextTime = now + rg.holdTime[rg.id];
	switch (rg.id) {
		case 0: rg.output = 0; break;		
		case 1: if(rg.output == 0) rg.output = rg.pattern[1]; else rg.output <<= 1; break;
		case 2: if(rg.output == 0) rg.output = rg.pattern[2]; else rg.output >>= 1; break;
	}
}

void Show::update() {
	if(dat != rg.output) return;
	dat = rg.output;
	for(uint8_t i = 0; i < 8; i++) {
		digitalWrite(rg.leds[i], (dat >> i) & 0x01);
	}
}

#endif	// BINGO_HPP

