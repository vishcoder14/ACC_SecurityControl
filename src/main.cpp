#include <Arduino.h>
#include <SPI.h>
#include <MFRC522.h> // library for MFRC522 RFID reader

#define SS_PIN 10 // pin for SS (slave select) on the MFRC522
#define RST_PIN 9 // pin for RST (reset) on the MFRC522
#define SMPS_TRIGG_PIN 3
#define CABIN_LIGHT_PIN 4
#define buzzerPin 7

MFRC522 rfid(SS_PIN, RST_PIN); // create an instance of the MFRC522

const byte AuthenticationCardUID[4] = {0xA5, 0x3E, 0x92, 0xAC}; // storing the authentication card's UID
const byte LightTriggCardUID[4] = {0xA, 0x3C, 0x90, 0xAF}; // storing the light trigg card's UID

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
}

void loop() {
  if(rfid.PICC_IsNewCardPresent()) { // check if a new card is present
    rfid.PICC_ReadCardSerial(); // read the card's UID
    
    Serial.print("Card UID: ");
    for(uint8_t abit=0; abit<rfid.uid.size; abit++){
    Serial.print(rfid.uid.uidByte[abit]);
    Serial.print("\t"); }
    
    // checking a match for authentication card UID with detect card UID:
    if(rfid.uid.uidByte[0] == AuthenticationCardUID[0] && rfid.uid.uidByte[1] == AuthenticationCardUID[1]
      && rfid.uid.uidByte[2] == AuthenticationCardUID[2] && rfid.uid.uidByte[3] == AuthenticationCardUID[3]) { 
      digitalWrite(SMPS_TRIGG_PIN, HIGH); // switch ON SMPS supply
    } 
    else {
      Serial.println("Access denied");

    }
  }
  delay(100);
}