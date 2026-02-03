#include "bingo.hpp"
// Konstanten
const uint8_t PIN_BUTTON     = GPIO_NUM_19;                     // Input-Button-Pin - LOW aktiv

// Globals
uint8_t buttonFlag = 0;

// Instanzen
Bingo_Button button(PIN_BUTTON);  // Button-Objekt

void setup() {
	Serial.begin(115200);
	delay(1000);  // Warte auf Serial-Monitor
	Serial.println("\nBingo gestartet.");
}

void loop() {
	button.update(buttonFlag);

	// Test Start
	if (buttonFlag) {
		Serial.println("Button gedrückt");
	}
	// Test Ende
	
	if(buttonFlag) {
		buttonFlag = 0;
		delay(500);  // Entprellzeit
	}
}