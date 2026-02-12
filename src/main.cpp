#include "bingo.hpp"

// Konfiguration
Config_t co = {
	.prMax = 3,
	.leds = { GPIO_NUM_18, GPIO_NUM_3, GPIO_NUM_4, GPIO_NUM_5, GPIO_NUM_13, GPIO_NUM_14, GPIO_NUM_16, GPIO_NUM_17 },
 	.btnPin = GPIO_NUM_19,
	.id = 0,
	.output = 0
 };

Button button(co);  																		// Button-Objekt mit Referenz auf Konfigurationsstruktur
Show show(co);                                          // Show-Objekt mit Referenz auf Konfigurationsstruktur
JobNext jobNext(1, co, 1000, 0b00000001);  				  		//Prozess-ID = 1, Referenz auf Konfigurationsstruktur, Haltezeit von 1000 ms und Initialwert 1
JobPrev jobPrev(2, co, 500, 0b10000000);  				  		//Prozess-ID = 2, Referenz auf Konfigurationsstruktur, Haltezeit von 500 ms und Initialwert 128

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
	jobNext.update(co.id);																					// Aktualisiere den Prozess-Job mit der aktuellen ID
	jobPrev.update(co.id);																					// Aktualisiere den Prozess-Job mit der aktuellen ID
	show.update(co);

	// Test Start
	test(co);
	// Test Ende

	delay(100);  // Loop-Delay
}