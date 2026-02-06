#include "bingo.hpp"

// Konstanten
const uint8_t PIN_BUTTON     = GPIO_NUM_19;                     // Input-Button-Pin - LOW aktiv
const uint8_t PROCESS_MAX    = 2;                               // Maximale Anzahl Prozesse

// Global
uint8_t buttonFlag = 0;																					// Button Down -> >0
uint8_t processID = 0;																					// 0 .. PROCESS_MAX: 0 -> Idle
uint8_t output = 0;																							// Bit 0..7 gesetzt -> HIGH

// Instanzen
Bingo_Button button(PIN_BUTTON);  															// Button-Objekt
Bingo_ProcessManager processManager(PROCESS_MAX);  							// Prozess-Manager-Objekt mit PROCESS_MAX + 1 Prozessen

// Funktionen
void checkNull(uint8_t &rOut, const uint8_t prID) { if(prID == 0) rOut = 0; }

// Main
void setup() {
	Serial.begin(115200);
	delay(1000);  // Warte auf Serial-Monitor
	Serial.println("\nBingo gestartet.");
}

void loop() {
	button.update(buttonFlag);
	processManager.update(processID, buttonFlag);
	checkNull(output, processID);

	// Test Start
	if (buttonFlag) {
		Serial.print("Prozess-ID: ");
		Serial.println(processID);
	}
	// Test Ende
	
	if(buttonFlag) {
		buttonFlag = 0;
		delay(200);  // Entprellzeit
	}
}