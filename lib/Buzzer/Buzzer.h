/*
[Part of ADVANCED SECURITY CONTROL ALGORITHM]

Buzzer sound control library
----------------------------

Arduino compatible library for external buzzers, 
contains several buzzer beep modes.

[vishnus_technologies (C) 2022]
------------------------------------------------
*/

#include <Arduino.h>

// connect buzzer signal pin to nano D6
#define buzzerPin 6

void normalBeep(uint16_t _freq, uint8_t _duration, uint16_t _interval);
void emergencyBeep(uint16_t _freq, uint8_t _duration, uint16_t _interval);
void criticalBeep(uint16_t _freq, uint8_t _duration, uint16_t _interval);

// normalBeep(uint16_t frequency, uint16_t delay(ms))
void normalBeep(uint16_t _freq, uint8_t _duration, uint16_t _interval) {
  if(_duration == 0) {
    _duration = 4;
  }
  for(uint8_t recc = 0; recc < _duration; recc ++) {
    tone(buzzerPin, _freq);
    delay(_interval);
    noTone(buzzerPin);
    delay(_interval);
  }
}

// emergencyBeep(uint16_t frequency, uint16_t delay(ms))
void emergencyBeep(uint16_t _freq, uint8_t _duration, uint16_t _interval) {
  if(_duration == 0) { 
  _duration = 7; 
  }
  for(uint8_t recc = 0; recc < _duration; recc ++) {
    tone(buzzerPin, _freq);
    delay(_interval);
    noTone(buzzerPin);
    delay(_interval);
  }
}

// criticalBeep(uint16_t frequency, uint16_t delay(ms))
void criticalBeep(uint16_t _freq, uint8_t _duration, uint16_t _interval) {
  if(_duration == 0) {
    _duration = 15; 
  }
  for(uint8_t recc = 0; recc < _duration; recc ++) {
    tone(buzzerPin, _freq);
    delay(_interval);
    noTone(buzzerPin);
    delay(_interval);
  }
}
