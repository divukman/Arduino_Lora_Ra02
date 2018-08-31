#include <SPI.h> // arduino spi library
#include <LoRa.h> // arduino libraries ra02 lora

int counter = 0;

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
    Serial.print("Sending... ");
    Serial.println(counter);

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
  Serial.println("Sending msg: " + msg);
   
  // send packet
  LoRa.beginPacket();
  LoRa.print(msg);
  LoRa.print(counter);
  LoRa.endPacket();
  
  counter++;
  Serial.println("Done...");
  Serial.println();
  
  delay(3000);

}
