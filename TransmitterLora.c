/*

The transmitter code arduino for sending data packet lora radio module 433MHz  ra02 lora

*/

#include <SPI.h> // arduino spi library
#include <LoRa.h> // arduino libraries ra02 lora

// Constants - We are sending only one character
//*********************************************************
const char * MSG_DEFAULT = "*";
const char * MSG_ONE = "A";
const char * MSG_TWO = "B";
const char * MSG_THREE = "C";

// BUTTONS SETUP
//*********************************************************
const int button1Pin = 8; 
int button1State = 0; 
const int button2Pin = 9; 
int button2State = 0;  
const int button3Pin = 10; 
int button3State = 0;    

// Message pointer
//*********************************************************
char *msg = 0;


// Setup function
void setup() {
	  Serial.begin(9600);
	  while (!Serial);
	  
	  
	  // Lora setup
	  Serial.println("LoRa Sender");
	  if (!LoRa.begin(433E6)) {
		Serial.println("Starting LoRa failed!");
		while (1);
	  }
	 LoRa.setSpreadingFactor(10);
	 LoRa.setSignalBandwidth(62.5E3);
	 LoRa.crc();
	 
	 
	// Init button input PINs
    pinMode(button1Pin, INPUT);
    pinMode(button1Pin, INPUT_PULLUP);
    pinMode(button2Pin, INPUT);
    pinMode(button2Pin, INPUT_PULLUP);
    pinMode(button3Pin, INPUT);
    pinMode(button3Pin, INPUT_PULLUP);
}

// Main loop function
void loop() {

  Serial.print("Arduino: Lora -> Sending packet: ");

   // Read input button PINs
  button1State = digitalRead(button1Pin);
  button2State = digitalRead(button2Pin);
  button3State = digitalRead(button3Pin);
   
    
  // React to button digital PIN state changes
  if (button1State == LOW) {
    msg = MSG_ONE;
  } else if (button2State == LOW) {
     msg = MSG_TWO;
  } else if (button3State == LOW) {
     msg = MSG_THREE;
  } else {
    msg = MSG_DEFAULT;
  }
  
  // Print some info to serial line
  Serial.print(msg);
  Serial.println("---");
   
   
  // Send packet
  LoRa.beginPacket();
  LoRa.print(msg);
  LoRa.endPacket();

  
  // Sleep 2 seconds
  delay(2000);

}