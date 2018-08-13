#include <Arduino.h>

int sensorPin = A0;
int sensorValue = 0;

double readVcc() {
  
  // AREF, 1.1V (V_BG)
  ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
  
  delay(2); // Wait for Vref to settle

  // start conversion
  ADCSRA |= _BV(ADSC);

  // wait conversion done
  while (bit_is_set(ADCSRA,ADSC));

  int value = ADCL;
  value |= ADCH<<8;

  // value is the 0-1023 reading of 1.1V seen from actual 0-Vcc
  // value/1.1 = 1023/vcc => vcc = 1.1 * 1023 / value
  
  return 1.1 * 1023 / value;
}

double VccAdjust = 1.0;

void setup() {  
  Serial.begin(115200);

  VccAdjust = 5.0 / readVcc();

  if (VccAdjust < 1.0) VccAdjust = 1.0;
}

void loop() {   
  sensorValue = analogRead(sensorPin);
  
  double sensorValueAdj = sensorValue / VccAdjust;

  int val = ((sensorValueAdj/1023.0)*255.0);

  // encode into 1 byte
  Serial.print((char)val);
  
  /*Serial.print("sensorValue=");Serial.println(sensorValue);
  Serial.print("sensorValueAdj=");Serial.println(sensorValueAdj);
  Serial.print("VccAdjust=");Serial.println(VccAdjust);
  Serial.print("val=");Serial.println(val);*/
}
