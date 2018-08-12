#include <Arduino.h>
#include <EtherCard.h>

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

#define BUF_SIZE 200
uint8_t *buf;

static byte mymac[] = { 0x1A,0x2B,0x3C,0x4D,0x5E,0x6F };
byte Ethernet::buffer[700];
static uint32_t timer;
const char srvname[] PROGMEM = "main.searchathing.com";
const int dstPort PROGMEM = 1234;

const int srcPort PROGMEM = 4321;

void setup() {  
  Serial.begin(115200);

  buf = (uint8_t *)malloc(BUF_SIZE);
  if (buf == 0) Serial.println("malloc failed");
   
  if (ether.begin(sizeof Ethernet::buffer, mymac, SS) == 0)
    Serial.println("eth error");
  if (!ether.dhcpSetup())
    Serial.println("dhcp error");

  ether.printIp("IP:  ", ether.myip);
  ether.printIp("GW:  ", ether.gwip);
  ether.printIp("DNS: ", ether.dnsip);

  if (!ether.dnsLookup(srvname))
    Serial.println("dns error");
    
  ether.printIp("SRV: ", ether.hisip);
}

int bufCnt=0;

void loop() { 
  sensorValue = analogRead(sensorPin);  
    
  buf[bufCnt++] = sensorValue/4;

  if (bufCnt == BUF_SIZE)
  {
    ether.sendUdp(buf, BUF_SIZE, srcPort, ether.hisip, dstPort );    
    bufCnt=0;
  } 
}
