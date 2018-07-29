#include <Arduino.h>

#define DEBUG

#ifdef DEBUG
 #define DEBUG_PRINT(x)     Serial.print (x)
 #define DEBUG_PRINTHEX(x)     Serial.print (x,HEX)
 //#define DEBUG_PRINTF(x, args...)     Serial.printf (x, args)
 #define DEBUG_PRINTLN(x)  Serial.println (x)
#else
 #define DEBUG_PRINT(x)
 //#define DEBUG_PRINTF(x, args...)
 #define DEBUG_PRINTHEX(x)
 #define DEBUG_PRINTLN(x)
#endif

int sensorPin = A0;
int sensorValue = 0;

void setup() {  
  Serial.begin(115200); 
}

void loop() { 
  sensorValue = analogRead(sensorPin);  

  // mangled word 
  //   0000CCBB 11AAAAAA
  // from original sensorValue
  //   ------CC BBAAAAAA

  // (transmitted LSB mode)
  //   11AAAAAA 0000CCBB

  uint8_t low = (sensorValue & 0x3f) | 0xc0;
  Serial.write(low);       
  
  uint8_t high = ((sensorValue & 0xc0) >> 6) | ((sensorValue >> 8) & 0xf);
  Serial.write(high);
/*
  Serial.print("value = "); Serial.println(sensorValue);
  Serial.print("low = "); Serial.println(low);
  Serial.print("high = "); Serial.println(high);  */
}
