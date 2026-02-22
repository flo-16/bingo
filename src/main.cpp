#include "bingo.hpp"

#define LONG_PRESS_TIME 1000  																		// Zeit in ms, die für einen Longclick benötigt wird

// Konfiguration
Config_t co = {
	.prMax = 3,
	.leds = { GPIO_NUM_18, GPIO_NUM_3, GPIO_NUM_4, GPIO_NUM_5, GPIO_NUM_13, GPIO_NUM_14, GPIO_NUM_16, GPIO_NUM_17 },
 	.btnPin = GPIO_NUM_19,
	.holdTime = { 20, 1000, 500 },
	.pattern = { 0, 0b00000001, 0b10000000 },
	.id = 0,
	.output = 0,
	.click = NOCLICK
 };

Button button(co, LONG_PRESS_TIME);  															// Button-Objekt mit Referenz auf Konfigurationsstruktur
Handler handler(co);  																						// Handler-Objekt mit Referenz auf Konfigurationsstruktur
Show show(co);                                          					// Show-Objekt mit Referenz auf Konfigurationsstruktur

void test(Config_t &rg) {
	uint8_t static var = 0;
	if(var != rg.output) {
		var = rg.output;
		Serial.println("Output: " + String(var));
	}
}

void setup() {
	Serial.begin(115200);
	delay(1000);  																									// Warte auf Serial-Monitor
	Serial.println("\nApp gestartet.");
}

void loop() {
	button.update();
	handler.update();
	show.update();
	test(co);
	delay(10);  																										// Kurze Pause, um CPU-Last zu reduzieren
}
