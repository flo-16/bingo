#include "bingo.hpp"

// Konfiguration
Config_t co = {
	.prMax = 2,
	.leds = { GPIO_NUM_18, GPIO_NUM_3, GPIO_NUM_4, GPIO_NUM_5, GPIO_NUM_13, GPIO_NUM_14, GPIO_NUM_16, GPIO_NUM_17 },
 	.btnPin = GPIO_NUM_19,
	.id = 0,
	.output = 0,
	.buttonFlag = 0,
	.bupressed = 0,
	.prState = NONE
};

// Konstanten
const uint8_t PIN_BUTTON     = GPIO_NUM_19;                     // Input-Button-Pin - LOW aktiv
const gpios_t PIN_LEDS = { .pin =
	{ GPIO_NUM_18, GPIO_NUM_3, GPIO_NUM_4, GPIO_NUM_5, GPIO_NUM_13, GPIO_NUM_14, GPIO_NUM_16, GPIO_NUM_17 }
};

const uint8_t PROCESS_MAX    = 2;                               // Maximale Anzahl Prozesse

// Global
uint8_t buttonFlag = 0;																					// Button Down -> >0
uint8_t processID = 0;																					// 0 .. PROCESS_MAX: 0 -> Idle
uint8_t output = 0;																							// Bit 0..7 gesetzt -> HIGH

// Instanzen
Button button(PIN_BUTTON);  																		// Button-Objekt
Manager processManager(PROCESS_MAX);  													// Prozess-Manager-Objekt mit PROCESS_MAX + 1 Prozessen
Show show(PIN_LEDS);                                            // Show-Objekt mit LED-Pins
Job_Next jobNext(1, output, 1000);  														// Prozess-ID = 1, Referenz auf output und Haltezeit von 1000 ms

// Funktionen
void checkNull(uint8_t &rOut, const uint8_t prID) { if(prID == 0) rOut = 0; }

// Main
void setup() {
	Serial.begin(115200);
	delay(1000);  																									// Warte auf Serial-Monitor
	show.init();  																									// Initialisiere die Show (LEDs)
	Serial.println("\nBingo gestartet.");
}

void loop() {
	button.update(buttonFlag);
	processManager.update(processID, buttonFlag);
	checkNull(output, processID);
	jobNext.update(output);
	// hier die Jobs aufrufen, z.B.:
	show.update(output);

	// Test Start
	if (buttonFlag) {
		Serial.print("Prozess-ID: ");
		Serial.println(processID);
		Serial.print("Output: ");
		Serial.println(output, BIN);
	}
	// Test Ende
	
	if(buttonFlag) {
		buttonFlag = 0;
		delay(200);  // Entprellzeit
	}
}