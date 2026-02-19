#ifndef BINGO_HPP
#define BINGO_HPP

#pragma once

#include <Arduino.h>

// Typen
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

#endif	// BINGO_HPP

