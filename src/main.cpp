/*
# main.cpp

ADVANCED SECURITY CONTROL ALGORITHM
-----------------------------------

[vishnus_technologies (C) 2023]
[10-FEBRUARY-2023 09:11AM]
-----------------------------------
*/

#include <Arduino.h>
#include <SPI.h>
#include <MFRC522.h>
#include <Buzzer.h>

/*
RFID PIN CONFIGURATION
+-----------------+
|  NANO  |  RFID  |
+-----------------+
| SDA/SS |  D10   |
| SCK    |  D13   |
| MOSI   |  D11   |
| MSOI   |  D12   |
| IRQ    |        |
| GND    |  GND   |
| RST    |  D9    |
| 3.3v   |  3.3v  |
+-----------------+
*/

#define SS_PIN 10
#define RST_PIN 9
#define SMPS_TRIGG_RPIN 2   // SMPS of 12v 27A
#define PS_TRIGG_RPIN 3     // Metal PS of 12v 5A
#define CABIN_LIGHT_RPIN 4
#define relayPin4 5

#define SWITCH_ON LOW
#define SWITCH_OFF HIGH

MFRC522 rfid(SS_PIN, RST_PIN);
MFRC522::MIFARE_Key key;

byte scanUID[4];  // store scanned card's UID 

/* AUTHENTICATION UIDs: */
// SMPS & Metal PS authentication UID: [int?= 16562146172] [hex?= A53E92AC]
const byte PSU_AuthUID[4] = {165, 62, 146, 172};
bool PSU_state = 0;
// Cabin light authenticaition UID: [int?= ] [hex?= ]
const byte Clight_AuthUID[4] = {0xA, 0x3C, 0x90, 0xAF};
bool Clight_state = 0;

int scanRFID();
bool IsArrayEqual(const byte a1[], byte a2[], uint8_t asize);

void setup() {
  Serial.begin(9600);   // starts serial COM @baud-9600

  pinMode(SMPS_TRIGG_RPIN, OUTPUT);
  pinMode(CABIN_LIGHT_RPIN, OUTPUT);
  pinMode(PS_TRIGG_RPIN, OUTPUT);
  pinMode(relayPin4, OUTPUT);
  pinMode(buzzerPin, OUTPUT);

  digitalWrite(SMPS_TRIGG_RPIN, SWITCH_OFF);
  digitalWrite(CABIN_LIGHT_RPIN, SWITCH_OFF);
  digitalWrite(PS_TRIGG_RPIN, SWITCH_OFF);
  digitalWrite(relayPin4, SWITCH_OFF);
  noTone(buzzerPin);
  delay(750);
  
  SPI.begin();
  rfid.PCD_Init();
  delay(750);
  Serial.print("Please scan your RFID card at the scanner");
}

void loop() {
scanRFID();
}

// starts RFID scanner
int scanRFID() {
  if( ! rfid.PICC_IsNewCardPresent()) { 
    return 1; }

  if( ! rfid.PICC_ReadCardSerial()) { 
    return 1; }
       
  // Reads RFID UID
  Serial.print("\nUID: ");
  for(int i = 0; i < rfid.uid.size; i++) {  
    scanUID[i] = rfid.uid.uidByte[i];
    Serial.print(scanUID[i], HEX);   // prints current card's UID.
    Serial.print(" ");
  }
  Serial.println();

  /* COMPARES & CHECK ALL AUTHENTICATION UIDs */
  // PSU power access authentication [switch ON/OFF]:
  if(IsArrayEqual(PSU_AuthUID, scanUID, rfid.uid.size)) {
    // triggers SMPS:
    if(digitalRead(SMPS_TRIGG_RPIN) == SWITCH_OFF or PSU_state == 0) {
      digitalWrite(SMPS_TRIGG_RPIN, SWITCH_ON);
      digitalWrite(PS_TRIGG_RPIN, SWITCH_ON);
      PSU_state = 1;
      Serial.println("[SMPS ON]");
      //normalBeep(3750, 2, 450);
    }
    else if(digitalRead(SMPS_TRIGG_RPIN) == SWITCH_ON or PSU_state == 1) {
      digitalWrite(SMPS_TRIGG_RPIN, SWITCH_OFF);
      digitalWrite(PS_TRIGG_RPIN, SWITCH_OFF);
      PSU_state = 0;
      Serial.println("[SMPS OFF]");
    }
  }

  // cabin light power access authentication [switch ON/OFF]: 
  if(IsArrayEqual(Clight_AuthUID, scanUID, rfid.uid.size)) {
    if(digitalRead(CABIN_LIGHT_RPIN) == SWITCH_OFF or Clight_state == 0) {
      digitalWrite(CABIN_LIGHT_RPIN, SWITCH_ON);
      Clight_state = 1;
      Serial.println("[Cabin light ON]");
    }
    else if(digitalRead(CABIN_LIGHT_RPIN) == SWITCH_ON or Clight_state == 1) {
      digitalWrite(CABIN_LIGHT_RPIN, SWITCH_OFF);
      Clight_state = 0;
      Serial.println("[Cabin light OFF]");
    }
  }

  rfid.PICC_HaltA();  // halts PICC reading process.
  return 0;
}

// compare arrays, return true if equal, false otherwise
bool IsArrayEqual(const byte a1[], byte a2[], uint8_t asize) {
  for(int i = 0; i < asize; i++) {
    if (a1[i] != a2[i]) {
      return false;
    } 
  }
  return true;
}