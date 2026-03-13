#ifndef BINGO_HPP
#define BINGO_HPP
#include <Arduino.h>

// Typdefinitionen
typedef struct {
	const uint8_t		leds[8];																						// LED-Pins
	const uint8_t		btn;																								// Button-Pin
	const uint16_t	longPress;																					// Zeit in ms, die für einen Longclick benötigt wird
	const uint8_t 	prMax;																							// Anzahl der Pattern
	const uint16_t 	hold[3];																						// Pausen zwischen den Mustern in ms
	const uint8_t 	pattern[3];																					// Ausgangsmuster für die Modi 1 und 2	
} rock_t;

typedef enum { NOCLICK = 0, SHORTCLICK, LONGCLICK } clickType_t;			// Klick-Status als Enumeration

typedef struct {
	uint8_t 				id;																									// Programm-Modus
	uint8_t 				ltClick; 																						// Anzahl der Longclicks	
	uint8_t 				output;																							// aktuelles Ausgangsmuster
	clickType_t 		click;																							// Click-Status
	const rock_t 		*rPtr;																							// Zeiger auf die globale Konstantenstruktur
} config_t;	

// Klassendefinitionen
class Button {
	private:
		config_t &rg;
	public:
		Button(config_t &rg);
		void update();
};

class Handler {
	private:
		config_t &rg;
		uint32_t nextTime;
	public:
		Handler(config_t &rg) : rg(rg), nextTime(0) {}
		void update();		
};

class Show {
	private:
		config_t &rg;
		uint8_t dat;
	public:
		Show(config_t &rg);
		void update();
};

// Implementierung der Methoden

Button::Button(config_t &rg) : rg(rg) {
	pinMode(rg.rPtr->btn, INPUT_PULLUP);
}

void Button::update() {
	static uint32_t nextTime = 0;
	static bool busy = false;
	if(busy) {
		if(millis() < nextTime) return;
		uint8_t c1 = digitalRead(rg.rPtr->btn);
		rg.click = (c1 == LOW) ? LONGCLICK : SHORTCLICK;
		while(digitalRead(rg.rPtr->btn) == LOW);  												// Warte auf Loslassen
		busy = false;
	} else {
		uint8_t c0 = digitalRead(rg.rPtr->btn);
		if(c0 == LOW) {
			busy = true;
			nextTime = millis() + rg.rPtr->longPress;  												// Warte auf möglichen Longclick
			delay(10);  																											// Entprellzeit
		}
		rg.click = NOCLICK;
	}
}	

void Handler::update() {
	if(rg.click == SHORTCLICK) {		
		rg.click = NOCLICK;		
		rg.id++;
		rg.id %= rg.rPtr->prMax;  																				// Modulo-Operation, um innerhalb der Anzahl der Pattern zu bleiben
	}
	uint32_t now = millis();
	if(now < nextTime) return;
	nextTime = now + rg.rPtr->hold[rg.id];
	switch (rg.id) {
		case 0: rg.output = 0; break;		
		case 1: if(rg.output == 0) rg.output = rg.rPtr->pattern[rg.id]; else rg.output <<= 1; break;
		case 2: if(rg.output == 0) rg.output = rg.rPtr->pattern[rg.id]; else rg.output >>= 1; break;
	}
}

Show::Show(config_t &rg) : rg(rg), dat(0) {
	for(uint8_t i = 0; i < 8; i++) {
		pinMode(rg.rPtr->leds[i], OUTPUT);
		digitalWrite(rg.rPtr->leds[i], LOW);
	}
}

void Show::update() {
	if(dat == rg.output) return;
	dat = rg.output;
	for(uint8_t i = 0; i < 8; i++) {
		digitalWrite(rg.rPtr->leds[i], (dat >> i) & 0x01);
	}
}

#endif	// BINGO_HPP

