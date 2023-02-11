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
#include <MFRC522.h> // library for MFRC522 RFID reader

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

#define SS_PIN 10 // pin for SS (slave select) on MFRC522
#define RST_PIN 9 // pin for RST (reset) on MFRC522
#define SMPS_TRIGG_RPIN 3
#define CABIN_LIGHT_RPIN 4
#define buzzerPin 7

#define SWITCH_ON LOW
#define SWITCH_OFF HIGH

MFRC522 rfid(SS_PIN, RST_PIN);
MFRC522::MIFARE_Key key;

byte scanCardUID[4]; 
bool authFlag = true;

// AUTHENTICATION UIDs:
/* SMPS authentication match UID: [int?= 16562146172] [hex?= A53E92AC] */
const byte SMPSAuthUID[4] = {165, 62, 146, 172};

/* Cabin light authenticaition UID: [int?= ] [hex?= ] */
const byte ClightAuthUID[4] = {0xA, 0x3C, 0x90, 0xAF};

int scanRFID();
bool IsArrayEqual(const byte a1[], byte a2[], uint8_t asize);

void setup() {
  Serial.begin(9600); // start serial communication.

  pinMode(SMPS_TRIGG_RPIN, OUTPUT);
  pinMode(CABIN_LIGHT_RPIN, OUTPUT);
  pinMode(buzzerPin, OUTPUT);

  digitalWrite(SMPS_TRIGG_RPIN, SWITCH_OFF);
  digitalWrite(CABIN_LIGHT_RPIN, SWITCH_OFF);

  noTone(buzzerPin);
  SPI.begin(); // start SPI communication.
  rfid.PCD_Init(); // initialize the MFRC522.
  delay(750);
  Serial.print("Please scan your RFID Card at the scanner");
}

void loop() {
scanRFID();
}

// starts scanner
int scanRFID() {
  if( ! rfid.PICC_IsNewCardPresent()) { 
    return 1; }

  if( ! rfid.PICC_ReadCardSerial()) { 
    return 1; }
       
  // Reads RFID UID and store
  Serial.print("\nUID: ");
  for(int i = 0; i < rfid.uid.size; i++) {  
    scanCardUID[i] = rfid.uid.uidByte[i];
    //Serial.print(rfid.uid.uidByte[i]);
    Serial.print(scanCardUID[i], HEX);    // prints current scanned card's UID.
    Serial.print(" ");
  }
  Serial.println();

  /* COMPARES & CHECK ALL AUTHENTICATION UIDs */
  // For SMPS power access authentication [switch ON]:
  if(IsArrayEqual(SMPSAuthUID, scanCardUID, rfid.uid.size)) {
    if(digitalRead(SMPS_TRIGG_RPIN) == SWITCH_OFF) {
      digitalWrite(SMPS_TRIGG_RPIN, SWITCH_ON);
      Serial.println("Access Granted: SMPS switched ON");
    }
    else if(digitalRead(SMPS_TRIGG_RPIN) == SWITCH_ON) {
      digitalWrite(SMPS_TRIGG_RPIN, SWITCH_OFF);
      Serial.println("[SMPS switched OFF]");
    }
  }

  // For cabin light power access [switch ON]: 
  if(IsArrayEqual(ClightAuthUID, scanCardUID, rfid.uid.size)) {
    if(digitalRead(CABIN_LIGHT_RPIN) == SWITCH_OFF) {
      digitalWrite(CABIN_LIGHT_RPIN, SWITCH_ON);
      Serial.println("Access Granted: Cabin light switched ON");
    }
    else if(digitalRead(CABIN_LIGHT_RPIN) == SWITCH_ON) {
      digitalWrite(CABIN_LIGHT_RPIN, SWITCH_OFF);
      Serial.println("[Cabin light switched OFF]");
    }
  }

  rfid.PICC_HaltA();  // halts PICC reading process.
  return 0;
}

// compare arrays , return true if equal, false otherwise
bool IsArrayEqual(const byte a1[], byte a2[], uint8_t asize) {
  for(int i = 0; i < asize; i++) {
    if (a1[i] != a2[i]) {
      return false;
    } 
  }
  return true;
}