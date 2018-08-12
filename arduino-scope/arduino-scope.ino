#include <Arduino.h>

int sensorPin = A0;
int sensorValue = 0;

// clear bit
#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif

// set bit
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif

void setup() {  
  Serial.begin(115200);
}

void loop() { 
  sensorValue = analogRead(sensorPin);

  char val = (((double)sensorValue/1023)*255);

  // encode into 1 byte
  Serial.print(val);
  //Serial.println(val);
}
