#include "bingo.hpp"

// Konfiguration
Config_t co = {
	.prMax = 3,
	.leds = { GPIO_NUM_18, GPIO_NUM_3, GPIO_NUM_4, GPIO_NUM_5, GPIO_NUM_13, GPIO_NUM_14, GPIO_NUM_16, GPIO_NUM_17 },
 	.btnPin = GPIO_NUM_19,
	.holdTime = { 0, 1000, 500 },
	.pattern = { 0, 0b00000001, 0b10000000 },
	.id = 0,
	.output = 0
 };

uint8_t f0(const uint16_t hold);
uint8_t f1(const uint16_t hold);
uint8_t f2(const uint16_t hold);
uint8_t (*funcArr[])(const uint16_t) = { f0, f1, f2 };  					// Array von Zeigern auf die Prozess-Funktionen
uint16_t holdTime[] = { 0, 1000, 500 };  													// Haltezeiten für die Prozess-Funktionen

Button button(co);  																							// Button-Objekt mit Referenz auf Konfigurationsstruktur
Handler handler(co);  																						// Handler-Objekt mit Referenz auf Konfigurationsstruktur
Show show(co);                                          					// Show-Objekt mit Referenz auf Konfigurationsstruktur

void test(Config_t &rg) {
	uint8_t static var = 0;
	if(var != rg.output) {
		var = rg.output;
		Serial.println("Output: " + String(var));
	}
}

// Main
void setup() {
	Serial.begin(115200);
	delay(1000);  																									// Warte auf Serial-Monitor
	show.init();  																									// Initialisiere die Show (LEDs)
	Serial.println("\nApp gestartet.");
}

void loop() {
	button.update(co);
	handler.update();

//	co.output = funcArr[co.id](holdTime[co.id]);										// Rufe die aktuelle Prozess-Funktion auf
	show.update(co);

	test(co);

	delay(100);  
}

// Implementierungen der Prozess-Funktionen

uint8_t f0(const uint16_t hold) { return 0b00000000; }

uint8_t f1(const uint16_t hold) {
	const uint8_t pattern = 0b00000001;  														// Initial-Muster
	static uint32_t nextTime = 0;
	static uint8_t data = 0;
	uint32_t now = millis();
	if(now > nextTime) {
		nextTime = now + hold;
		if(data == 0) data = pattern; else data <<= 1;
	}
	return data; 
}

uint8_t f2(const uint16_t hold) {
	const uint8_t pattern = 0b10000000;  														// Initial-Muster
	static uint32_t nextTime = 0;
	static uint8_t data = 0;
	uint32_t now = millis();
	if(now > nextTime) {
		nextTime = now + hold;
		if(data == 0) data = pattern; else data >>= 1;
	}
	return data; 
}