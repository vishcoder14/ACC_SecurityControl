#include <Arduino.h>
#include <SPI.h>
#include <MFRC522.h> // library for MFRC522 RFID reader

#define SS_PIN 10 // pin for SS (slave select) on the MFRC522
#define RST_PIN 9 // pin for RST (reset) on the MFRC522
#define SMPS_TRIGG_PIN 3
#define CABIN_LIGHT_PIN 4
#define buzzerPin 7

MFRC522 rfid(SS_PIN, RST_PIN);
MFRC522::MIFARE_Key key;

byte readCardUID[4]; 
const byte AuthenticationUID[4] = {0xA5, 0x3E, 0x92, 0xAC}; // storing the authentication card's UID
const byte LightTriggUID[4] = {0xA, 0x3C, 0x90, 0xAF}; // storing the light trigg card's UID
int getID();

void setup() {
  Serial.begin(9600); // start serial communication

  pinMode(SMPS_TRIGG_PIN, OUTPUT);
  pinMode(CABIN_LIGHT_PIN, OUTPUT);
  pinMode(buzzerPin, OUTPUT);

  digitalWrite(SMPS_TRIGG_PIN, LOW);
  digitalWrite(CABIN_LIGHT_PIN, LOW);

  noTone(buzzerPin);
  SPI.begin(); // start SPI communication
  rfid.PCD_Init(); // initialize the MFRC522
  delay(750);
  Serial.println("please scan your RFID Card...");
}

void loop() {
getID();
}

int getID() // Function that will read and print the RFID cards UID.
{
  if( ! rfid.PICC_IsNewCardPresent()) { 
    return 1; }

  if( ! rfid.PICC_ReadCardSerial()) { 
    return 1; }
    
    Serial.print("UID: ");    
    for (int i = 0; i < rfid.uid.size; i++) {  
      readCardUID[i] = rfid.uid.uidByte[i];    // Reads RFID cards UID.
      Serial.print(readCardUID[i], HEX);    // Prints RFID cards UID to the serial monitor.
      Serial.print(" ");     
    }
    Serial.println();
    rfid.PICC_HaltA();     // Stops the reading process.
    return 0;
}