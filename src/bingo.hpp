#ifndef BINGO_HPP
#define BINGO_HPP
#include <Arduino.h>

// Globale Konstanten - Pin- und Zeitdefinitionen
const uint8_t LEDS[] = { GPIO_NUM_18, GPIO_NUM_3, GPIO_NUM_4, GPIO_NUM_5, GPIO_NUM_13, GPIO_NUM_14, GPIO_NUM_16, GPIO_NUM_17 };
const uint8_t BTN_PIN = GPIO_NUM_19;																// Button-Pin				
const uint16_t LONG_PRESS_TIME = 400;  															// Zeit in ms, die für einen Longclick benötigt wird
const uint8_t PR_MAX = 3;																						// Anzahl der Pattern
const uint16_t HOLDTIME[] = { 20, 1000, 500 };											// Pausen zwischen den Mustern in ms
const uint8_t PATTERN[] = { 0, 0b00000001, 0b10000000 };						// Ausgangsmuster für die Modi 1 und 2

// Typdefinitionen
typedef enum { NOCLICK = 0, SHORTCLICK, LONGCLICK } clickType_t;		
typedef struct {
	uint8_t id;																												// Programm-Modus
	uint8_t ltClick; 																									// Anzahl der Longclicks	
	uint8_t output;																										// aktuelles Ausgangsmuster
	clickType_t click;																								// Click-Status
} config_t;	

// Klassendefinitionen
class Button {
	private:
		config_t &rg;
		const uint16_t longPressTime;
	public:
		Button(config_t &rg, uint16_t lt);
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

Button::Button(config_t &rg, uint16_t lt) : rg(rg), longPressTime(lt) {
	pinMode(BTN_PIN, INPUT_PULLUP);
}

void Button::update() {
	static uint32_t nextTime = 0;
	static bool busy = false;
	if(busy) {
		if(millis() < nextTime) return;
		uint8_t c1 = digitalRead(BTN_PIN);
		rg.click = (c1 == LOW) ? LONGCLICK : SHORTCLICK;
		while(digitalRead(BTN_PIN) == LOW);  																		// Warte auf Loslassen
		busy = false;
	} else {
		uint8_t c0 = digitalRead(BTN_PIN);
		if(c0 == LOW) {
			busy = true;
			nextTime = millis() + longPressTime;  																	// Warte auf möglichen Longclick
			delay(10);  																														// Entprellzeit
		}
		rg.click = NOCLICK;
	}
}	

void Handler::update() {
	if(rg.click == SHORTCLICK) {		
		rg.click = NOCLICK;		
		rg.id++;
		rg.id %= PR_MAX;
	}
	uint32_t now = millis();
	if(now < nextTime) return;
	nextTime = now + HOLDTIME[rg.id];
	switch (rg.id) {
		case 0: rg.output = 0; break;		
		case 1: if(rg.output == 0) rg.output = PATTERN[1]; else rg.output <<= 1; break;
		case 2: if(rg.output == 0) rg.output = PATTERN[2]; else rg.output >>= 1; break;
	}
}

Show::Show(config_t &rg) : rg(rg), dat(0) {
	for(uint8_t i = 0; i < 8; i++) {
		pinMode(LEDS[i], OUTPUT);
		digitalWrite(LEDS[i], LOW);
	}
}

void Show::update() {
	if(dat != rg.output) return;
	dat = rg.output;
	for(uint8_t i = 0; i < 8; i++) {
		digitalWrite(LEDS[i], (dat >> i) & 0x01);
	}
}

#endif	// BINGO_HPP

