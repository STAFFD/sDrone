#include "MS561101BA.h"

MS561101BA presureSensor;

void setup(){
  Serial.begin(57600);
  presureSensor.init(0x77);
}

void loop(){
  
  float presure = presureSensor.getPressure(MS561101BA_OSR_256);
  Serial.println(presure);
  delay(100);
}
