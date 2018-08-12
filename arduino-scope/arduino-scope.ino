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

  // div 4 (010)
/*  cbi(ADCSRA, ADPS2);
  sbi(ADCSRA, ADPS1);
  cbi(ADCSRA, ADPS0);

  // enable adc conversion
  sbi(ADCSRA, ADEN);*/
}

/*
int analogRead2(int pin)
{
  if (pin >= 14) pin -= 14;
  
  ADMUX = (1<<6) | (pin & 0x07) | 1<<4; // set pin and enable left adjust

  // start conversion
  sbi(ADCSRA, ADSC);

  while (bit_is_set(ADCSRA, ADSC));
  
  int low  = ADCL;
  int high = ADCH;

  return (high <<8) | low;
}*/

void loop() { 
  sensorValue = analogRead(sensorPin);

  // encode into 1 byte
  Serial.print((char)(sensorValue/4));
}
