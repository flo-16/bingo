#include "bingo.hpp"

// Konfiguration
Config_t co = {
	.prMax = 2,
	.leds = { GPIO_NUM_18, GPIO_NUM_3, GPIO_NUM_4, GPIO_NUM_5, GPIO_NUM_13, GPIO_NUM_14, GPIO_NUM_16, GPIO_NUM_17 },
 	.btnPin = GPIO_NUM_19,
	.id = 0,
	.output = 0,
	.buttonFlag = 0,
	.bupressed = 0
};

Button button(co);  																		// Button-Objekt mit Referenz auf Konfigurationsstruktur
Manager manager(co);  																	// Prozess-Manager-Objekt mit Referenz auf Konfigurationsstruktur
Show show(co);                                          // Show-Objekt mit Referenz auf Konfigurationsstruktur
JobNext jobNext(1, co, 1000, 0b00000001);  				  		//Prozess-ID = 1, Referenz auf Konfigurationsstruktur, Haltezeit von 1000 ms und Initialwert 0xFF

// Main
void setup() {
	Serial.begin(115200);
	delay(1000);  																									// Warte auf Serial-Monitor
	show.init();  																									// Initialisiere die Show (LEDs)
	Serial.println("\nBingo gestartet.");
}

void loop() {
	button.update(co);
	manager.update(co);
	jobNext.update(co.id);																					// Aktualisiere den Prozess-Job mit der aktuellen ID
	show.update(co);

	// Test Start
	if (co.buttonFlag == 1) {
		Serial.println("ID: " + String(co.id) + " Output: " + String(co.output)); delay(100); 					// Debug-Ausgabe
	}
	// Test Ende

	if(co.buttonFlag == 1) {
		co.buttonFlag = 0;
		delay(200);  // Entprellzeit
	}
	delay(100);  // Loop-Delay
}