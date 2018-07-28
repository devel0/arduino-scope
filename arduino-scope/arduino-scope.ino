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
  Serial.println(sensorValue, HEX);
}
