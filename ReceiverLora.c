/*

The receiver code arduino for receiving data using lora radio module 433MHz.

*/

#include <SPI.h> // arduino spi library
#include <LoRa.h> // arduino libraries for ra02 lora

const int MY_OUTPUT = 4; // OUTPUT


// Setup function
void setup() {

    pinMode(MY_OUTPUT, OUTPUT);
  
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
    char firstCharacter = 0;
    String receivedBuffer = String();
    
    int packetSize = LoRa.parsePacket();
    if (packetSize) {
      // read packet
      while (LoRa.available()) {
        received = (char)LoRa.read();
        receivedBuffer += received;
        Serial.print(received);
      }
      
      Serial.println();
      Serial.println("Dobijo od transmittera string: " + receivedBuffer);
      firstCharacter = receivedBuffer.charAt(0);
      Serial.print("Prvi karakter oznacava botun: ");
      Serial.print(firstCharacter);
      Serial.println();
     


      Serial.print("Jos sam ziv...");
      

      // We are receiving only one character
      if (firstCharacter == 'A') {
          Serial.println("Received state: Button A");
          digitalWrite(MY_OUTPUT, HIGH); // Blinkaj prijo
          delay(1000);
          Serial.println("Blinknuo sam i gasim sada.");
          digitalWrite(MY_OUTPUT, LOW);
      } else if (firstCharacter == 'B') {
          Serial.println("Received state: Button B");
      } else if (firstCharacter == 'C') {
          Serial.println("Received state: Button C");
      } else if (firstCharacter == '-'){
          Serial.println("Received state: Idle (-)");
          digitalWrite(MY_OUTPUT, LOW);
      } else {
          Serial.println("Error! Received unknown character!");
      } 


       Serial.println("Sending confirmation");
      // send packet
      LoRa.beginPacket();
      LoRa.print(firstCharacter);
      LoRa.endPacket();
   }
}