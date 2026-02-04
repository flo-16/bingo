#include "bingo.hpp"
// Konstanten
const uint8_t PIN_BUTTON     = GPIO_NUM_19;                     // Input-Button-Pin - LOW aktiv
const uint8_t PROCESS_MAX    = 3;                               // Maximale Anzahl Prozesse
// Globals
uint8_t buttonFlag = 0;
uint8_t processID = 0;

// Instanzen
Bingo_Button button(PIN_BUTTON);  															// Button-Objekt
Bingo_ProcessManager processManager(PROCESS_MAX);  							// Prozess-Manager-Objekt mit 3 Prozessen

void setup() {
	Serial.begin(115200);
	delay(1000);  // Warte auf Serial-Monitor
	Serial.println("\nBingo gestartet.");
}

void loop() {
	button.update(buttonFlag);
	processManager.update(processID, buttonFlag);
	// Test Start
	if (buttonFlag) {
		Serial.println("Button gedrückt");
		Serial.print("Prozess-ID: ");
		Serial.println(processID);
	}
	// Test Ende
	
	if(buttonFlag) {
		buttonFlag = 0;
		delay(500);  // Entprellzeit
	}
}