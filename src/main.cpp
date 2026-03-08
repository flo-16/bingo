#include "bingo.hpp"

config_t co = {																										// globale Konfigurationsstruktur mit allen relevanten Parametern
	.id = 0,
	.ltClick = 0,
	.output = 0,
	.click = NOCLICK
 };

Button button(co, LONG_PRESS_TIME);  															// Button-Objekt mit Referenz auf Konfigurationsstruktur
Handler handler(co);  																						// Handler-Objekt mit Referenz auf Konfigurationsstruktur
Show show(co);                                          					// Show-Objekt mit Referenz auf Konfigurationsstruktur

void test(config_t &rg);																					// Testfunktion, um Änderungen an der Konfiguration zu überwachen.

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

void test(config_t &rg) {
	uint8_t static var = 0;
	if(var != rg.output) {
		var = rg.output;
		Serial.println("Output: " + String(var));
	}
	if(rg.click == LONGCLICK) {
		rg.click = NOCLICK; 
		uint8_t temp = rg.ltClick + 1;  															// Longclick incrementieren
		Serial.println("Long-Clicks: " + String(temp));								// um delay zu vermeiden, wird der Zähler inkrementiert und nicht direkt im Handler
		rg.ltClick = temp;  																					// Longclick-Zähler zurücksetzen
	}
}

