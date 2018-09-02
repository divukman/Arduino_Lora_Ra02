#include <SPI.h> // arduino spi library
#include <LoRa.h> // arduino libraries ra02 lora

/**
 * Transmitter module:
 * -  Send character, wait for confirmation. If no confirmation, resend and loop.
 */


 // Setup global variables, structures, pointers, etc...
 typedef struct {
   const char * NO_MSG = ""; // const to make cpp compiler happy
   const char * MSG_A = "A";
   const char * MSG_B = "B";
   const char * MSG_C = "C"; 
 } Messages;
 
  
 typedef struct {
   int PIN;
   char * MESSAGE;
   int state;
 } Button;
 

 
 Messages MESSAGES;
 char * LAST_MESSAGE = (char *) MESSAGES.NO_MSG; // const char * to char * type cast to make cpp compiler happy
 
 Button * BTN_A = 0;
 Button * BTN_B = 0;
 Button * BTN_C = 0;

 
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


 
 BTN_A = (Button *) malloc (sizeof(Button));
 BTN_B = (Button *) malloc (sizeof(Button));
 BTN_C = (Button *) malloc (sizeof(Button));
 
 BTN_A->PIN = 5;
 BTN_A->MESSAGE = (char *) MESSAGES.MSG_A;
 BTN_A->state = 0;

 BTN_B->PIN = 6;
 BTN_B->MESSAGE = (char *) MESSAGES.MSG_B;
 BTN_B->state = 0;
  
 BTN_C->PIN = 7;
 BTN_C->MESSAGE = (char *) MESSAGES.MSG_C;
 BTN_C->state = 0;
 
   
  // Init button input PINs
  pinMode(BTN_A->PIN, INPUT);
  pinMode(BTN_A->PIN, INPUT_PULLUP);
  pinMode(BTN_B->PIN, INPUT);
  pinMode(BTN_B->PIN, INPUT_PULLUP);
  pinMode(BTN_C->PIN, INPUT);
  pinMode(BTN_C->PIN, INPUT_PULLUP);
}



// Main loop function
void loop() {
  // Print current state to serial
  String infoMsg = strlen(LAST_MESSAGE) > 0 ? "No message to send!" : "Retrying last message!";
  Serial.println(infoMsg);
  
  
  // Setup message for transmission
  readButtonsAndSetMessage();

  
  // Transmit if message exists
  if (strlen(LAST_MESSAGE) > 0) { 
    doTransmit();
  }
}



/**
*   Reads the button states and sets the LAST_MESSAGE global variable to appropriate value.
* If no button was pushed, it reuses the LAST_MESSAGE (retransmit last value or do nothing).
*/
void readButtonsAndSetMessage() { 
  // Read input button PINs
  BTN_A->state = digitalRead(BTN_A->PIN);
  BTN_B->state = digitalRead(BTN_B->PIN);
  BTN_C->state = digitalRead(BTN_C->PIN);
  
  // React to button digital PIN state changes (ie check if button was pushed)
  
  if (BTN_A->state == LOW) {
    LAST_MESSAGE = BTN_A->MESSAGE;
  } else if (BTN_B->state == LOW) {
    LAST_MESSAGE = BTN_B->MESSAGE;
  } else if (BTN_C->state == LOW) {
    LAST_MESSAGE = BTN_C->MESSAGE;
  } else {
    // If no button is pushed, retry last message if its transmission has failed, otherwise go idle (put last message to blank)
    LAST_MESSAGE = strlen(LAST_MESSAGE) > 0 ? LAST_MESSAGE : (char *) MESSAGES.NO_MSG;
  }
  
}

/**
* Transmit the LAST_MESSAGE and wait for confirmation. 
* If confirmation arrives within 5 seconds, set LAST_MESSAGE to blank (idle).
*/
void doTransmit() {
  if (strlen(LAST_MESSAGE) > 0) {
  // Print some info to serial line
    Serial.print("Sending msg: ");
    Serial.print(LAST_MESSAGE);
    Serial.println();
     
    // send packet
    LoRa.beginPacket();
    LoRa.print(LAST_MESSAGE);
    LoRa.endPacket();

    Serial.println("Done...");
    ////////////////////////////////////////////////////////////////////////////

    
    Serial.println("Switching to receiver mode, listening for confirmation");
    int timeout = 5;
    char received = 0;  
    String receivedBuffer = String();
    
    while (timeout-- > 0) {
      int packetSize = LoRa.parsePacket();
      if (packetSize) { // Packet received
        // read packet
        while (LoRa.available()) {
          received = (char)LoRa.read();
          receivedBuffer += received;
          Serial.print(received);
         } 

        Serial.println();
        Serial.println("Confirmation received: " + receivedBuffer);
        
        char * cstr = receivedBuffer.c_str();
               
        if (strncmp(cstr, LAST_MESSAGE, 1)) { // If received message matches sent message.                 
        LAST_MESSAGE = MESSAGES.NO_MSG; // Set last message to blank (ie go idle)
        break;
        }
      } else { // No packet received, wait 1 second
        delay(1000);
      }
    }
    delay(1000);
  } 
}
