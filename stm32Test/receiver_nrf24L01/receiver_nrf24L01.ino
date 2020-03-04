/*
 * NRF24L01       Arduino_ Uno    Arduino_Mega    Blue_Pill(stm32f01C)
 * __________________________________________________________________________
 * VCC        |       3.3v      |     3.3v      |     3.3v
 * GND        |       GND       |     GND       |      GND
 * CSN        |   Pin10 SPI/SS  | Pin10 SPI/SS  |     B12 NSS1 (PA4) 3.3v
 * CE         |   Pin9          | Pin9          |     PA8 digital (PB0) 3.3v
 * SCK        |   Pin13         | Pin52         |     B13 SCK1   (PA5) 3.3v
 * MOSI       |   Pin11         | Pin51         |     B15 MOSI1  (PA7) 3.3v
 * MISO       |   Pin12         | Pin50         |     B14 MISO1  (PA6) 3.3v
 * 
 * 
 */

#include <RF24-STM.h>

#define IMU_SCALE                       32768
#define GYRO_SCALE                      2000
#define ANGLE_SCALE                     180
#define ANGLE_ROLL_CALIBRATE_VALUE      0.16
#define ANGLE_PITCH_CALIBRATE_VALUE     0.03
RF24 radio(PA8, PB12); // CE, CSN on Blue Pill
const uint64_t address = 0xF0F0F0F0E1LL;

uint8_t buffer[6];
void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.openReadingPipe(0, address);   //Setting the address at which we will receive the data
  radio.setPALevel(RF24_PA_MIN);       //You can set this as minimum or maximum depending on the distance between the transmitter and receiver.
  radio.setDataRate(RF24_2MBPS);
  radio.startListening();              //This sets the module as receiver
}
void loop(){
  if (radio.available()){              //Looking for the data

    
    radio.read(&buffer, sizeof(buffer));    //Reading the data

    Serial.print("Recv -> ");
    Serial.print("gyroX: ");
    Serial.print(gyroConvert(buffer[0], true));
    Serial.print(" gyroY: ");
    Serial.print(gyroConvert(buffer[1], true));
    Serial.print(" gyroZ: ");
    Serial.print(gyroConvert(buffer[2], true));
    Serial.print(" angleX: ");
    Serial.print(angleConvert(buffer[3], 0, true));
    Serial.print(" angleY: ");
    Serial.print(angleConvert(buffer[4], 0, true));
    Serial.print(" angleZ: ");
    Serial.print(angleConvert(buffer[5], 0, true));
    Serial.print(" ");
        
    Serial.println("End");
  }
delay(5);
}


float gyroConvert(short data, bool reverse){
  if (reverse) return (float)data / IMU_SCALE * GYRO_SCALE * -1;
  else return (float)data / IMU_SCALE * GYRO_SCALE;
}

float angleConvert(short data, float adjust, bool reverse){
  if(reverse) return ((float)data / IMU_SCALE * ANGLE_SCALE + adjust) * -1;
  else return (float)data / IMU_SCALE * ANGLE_SCALE + adjust;
}

float GPS_Convert(long data){
  return (float)(data / 10000000) + (float)(data % 10000000) / 100000.0 / 60.0;
}
