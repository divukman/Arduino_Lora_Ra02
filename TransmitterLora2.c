#include <SPI.h> // arduino spi library
#include <LoRa.h> // arduino libraries ra02 lora

/**
 * Transmitter module:
 * -  Send character, wait for confirmation. If no confirmation, resend and loop.
 */
 
int counter = 0;

int MODULE_ACTIVE = 1;
int CONFIRMATION_RECEIVED = 0;
String LAST_MESSAGE = "";

// Messages
const String MSG_DEFAULT = "-";
const String MSG_ONE = "A";
const String MSG_TWO = "B";
const String MSG_THREE = "C";
String msg;


// Buttons
const int button1Pin = 5; 
int button1State = 0; 
const int button2Pin = 6; 
int button2State = 0;  
const int button3Pin = 7; 
int button3State = 0;    


// Setup function
void setup() {
    Serial.begin(9600);
    while (!Serial);
    
    // Lora setup
    Serial.println("LoRa Transmitter");
    
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
    String infoMsg = MODULE_ACTIVE ? "MODULE IS ACTIVE" : "MODULE IS INACTIVE";
    Serial.println(infoMsg);
   

   // Read input button PINs
  button1State = digitalRead(button1Pin);
  button2State = digitalRead(button2Pin);
  button3State = digitalRead(button3Pin);


   
    
  // React to button digital PIN state changes
  if (button1State == LOW) {
    msg = MSG_ONE;
    MODULE_ACTIVE = 1;
  } else if (button2State == LOW) {
     msg = MSG_TWO;
     MODULE_ACTIVE = 1;
  } else if (button3State == LOW) {
     msg = MSG_THREE;
     MODULE_ACTIVE = 1;
  } else {
    msg = LAST_MESSAGE.length() > 0 ? LAST_MESSAGE : MSG_DEFAULT;
    MODULE_ACTIVE = LAST_MESSAGE.length() > 0;
  }



  
  if (MODULE_ACTIVE || !CONFIRMATION_RECEIVED) {
    // Print some info to serial line
      Serial.println("Sending msg: " + msg);
       
      // send packet
      LoRa.beginPacket();
      LoRa.print(msg);
      LoRa.endPacket();
    
      LAST_MESSAGE = msg;
      
      Serial.println("Done...");
      ////////////////////////////////////////////////////////////////////////////
    
      
      Serial.println("Switching to receiver mode, listening for confirmation");
      int timeout = 5;
      char received = 0;
      char firstCharacter = 0;
      String receivedBuffer = String();
      
      while (timeout-- > 0) {
            int packetSize = LoRa.parsePacket();
            if (packetSize) {
                // read packet
                while (LoRa.available()) {
                  received = (char)LoRa.read();
                  receivedBuffer += received;
                  Serial.print(received);
               } 
    
              Serial.println();
              Serial.println("Confirmation received: " + receivedBuffer);
              firstCharacter = receivedBuffer.charAt(0);
              if (firstCharacter == LAST_MESSAGE.charAt(0)) { 
                CONFIRMATION_RECEIVED = 1;
                LAST_MESSAGE = "";
                break;
              }
            } else {
                delay(1000);
            }
      }
      delay(1000);
  }
  
}