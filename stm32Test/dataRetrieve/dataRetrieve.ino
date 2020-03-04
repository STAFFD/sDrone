#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <JY901.h>

#define IMU_SCALE                       32768
#define GYRO_SCALE                      2000
#define ANGLE_SCALE                     180
#define ANGLE_ROLL_CALIBRATE_VALUE      0.16
#define ANGLE_PITCH_CALIBRATE_VALUE     0.03

RF24 radio(PA8, PB12); // CE, CSN on Blue Pill
const uint64_t address = 0xF0F0F0F0E1LL;
float gyro_pitch_input, gyro_roll_input, gyro_yaw_input;
float angle_pitch, angle_roll, angle_yaw;
uint32_t timer, t;

void setup() {
  Serial.begin(9600);
  gyro_setup();
  radio.begin();                  //Starting the Wireless communication
  radio.openWritingPipe(address); //Setting the address where we will send the data
  radio.setPALevel(RF24_PA_MIN);  //You can set it as minimum or maximum depending on the distance between the transmitter and receiver.
  radio.setDataRate(RF24_2MBPS);
  radio.stopListening();          //This sets the module as transmitter
}

void loop() {
  gyro_signalen();
  short text = 0x10;
//  sprintf (text, "ap:1.33", angle_pitch);
  
  timer = micros();
  radio.writeFast(&text, sizeof(text));                  //Sending the message to receiver
  radio.txStandBy();
  t = micros() - timer;
  Serial.println(t);

  short data = 32768;
  
  timer = micros();
  radio.writeFast(&data, sizeof(data));                  //Sending the message to receiver
  radio.txStandBy();
  t = micros() - timer;
  Serial.println(t);
}

void gyro_setup(void){
  Serial2.begin(9600);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//This part reads the raw gyro and accelerometer data from the GY901
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void gyro_signalen(void) {
  while (Serial2.available()) JY901.CopeSerialData(Serial2.read()); //Call JY901 data cope function
  gyro_pitch_input  = gyroConvert((float)JY901.stcGyro.w[0], true);
  gyro_roll_input = gyroConvert((float)JY901.stcGyro.w[1], true);
  gyro_yaw_input   = gyroConvert((float)JY901.stcGyro.w[2], true);
  
  angle_pitch  = angleConvert(JY901.stcAngle.Angle[0], ANGLE_ROLL_CALIBRATE_VALUE, true);
  angle_roll = angleConvert(JY901.stcAngle.Angle[1], ANGLE_PITCH_CALIBRATE_VALUE, true);
  angle_yaw = angleConvert(JY901.stcAngle.Angle[2], 0, true);
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
