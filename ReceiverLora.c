/*

The receiver code arduino for receiving data using lora radio module 433MHz.

*/

#include <SPI.h> // arduino spi library
#include <LoRa.h> // arduino libraries for ra02 lora


// Constants
//*********************************************************
const char * MSG_DEFAULT = "DEFAULT MSG";
const char * MSG_ONE = "BTN1";
const char * MSG_TWO = "BTN2";
const char * MSG_THREE = "BTN3";


// Setup function
void setup() {
	  // Serial setup
	  Serial.begin(9600);
	  while (!Serial);

	  // Lora setup
	  Serial.println("Lora Receiver");
	  if (!LoRa.begin(433E6)) {
		Serial.println("Starting LoRa failed ");
		while (1);
	  }
	 LoRa.setSpreadingFactor(10);
	 LoRa.setSignalBandwidth(62.5E3);
	 LoRa.crc();
}

// Main loop function
void loop() {
	  char received = 0;
	  
	  int packetSize = LoRa.parsePacket();
	  if (packetSize) {
		// read packet
		while (LoRa.available()) {
			received = (char)LoRa.read();
			Serial.print(received);
		}
		Serial.println("'");
		
		int STATE = LOW;
		
		// We are receiving only one character
		if (received == 'A') {
			  Serial.println('Received state: Button A');
			  STATE = HIGH;
		} else if (received == 'B') {
			  Serial.println('Received state: Button B');
		} else if (received == 'C') {
			  Serial.println('Received state: Button C');
		} else if (received == '-'){
			  Serial.println('Received state: Idle (-)');
		} else {
			  Serial.println('Error! Received unknown character!');
		} 
		
		digitalWrite(LED_BUILTIN, STATE);
	 }
}