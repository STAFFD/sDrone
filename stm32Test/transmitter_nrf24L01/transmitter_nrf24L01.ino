/*
 * NRF24L01       Arduino_ Uno    Arduino_Mega    Blue_Pill(stm32f01C)
 * __________________________________________________________________________
 * VCC        |       3.3v      |     3.3v      |     3.3v
 * GND        |       GND       |     GND       |      GND
 * CSN        |   Pin8  SPI/SS  | Pin10 SPI/SS  |     B12 NSS1 (PA4) 3.3v
 * CE         |   Pin7          | Pin9          |     PA8 digital (PB0) 3.3v
 * SCK        |   Pin13         | Pin52         |     B13 SCK1   (PA5) 3.3v
 * MOSI       |   Pin11         | Pin51         |     B15 MOSI1  (PA7) 3.3v
 * MISO       |   Pin12         | Pin50         |     B14 MISO1  (PA6) 3.3v
 * 
 * 
 */
#include <RF24-STM.h>
#include <JY901.h>

short gyro_pitch_input, gyro_roll_input, gyro_yaw_input;
short angle_pitch, angle_roll, angle_yaw;
RF24 radio(PA8, PB12); // CE, CSN    
const uint64_t address = 0xF0F0F0F0E1LL;
uint32_t timer;
uint8_t buffer[6];

void setup() {
Serial.begin(9600);
gyro_setup();
radio.begin();                  //Starting the Wireless communication
radio.openWritingPipe(address); //Setting the address where we will send the data
radio.setPALevel(RF24_PA_MIN);  //You can set it as minimum or maximum depending on the distance between the transmitter and receiver.
radio.setDataRate(RF24_2MBPS);
radio.stopListening();          //This sets the module as transmitter
}

void loop(){

  gyro_signalen();

  
  buffer[0] = gyro_pitch_input;
  buffer[1] = gyro_roll_input;
  buffer[2] = gyro_yaw_input;
  buffer[3] = angle_pitch;
  buffer[4] = angle_roll;
  buffer[5] = angle_yaw;
    

//  Serial.println(text);
  timer = micros();
  radio.write(&buffer, sizeof(buffer));                  //Sending the message to receiver
  Serial.println(micros() - timer);

}

void gyro_setup(void){
  Serial2.begin(115200);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//This part reads the raw gyro and accelerometer data from the GY901
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void gyro_signalen(void) {
  while (Serial2.available()) JY901.CopeSerialData(Serial2.read()); //Call JY901 data cope function
  gyro_pitch_input = JY901.stcGyro.w[0];
  gyro_roll_input = JY901.stcGyro.w[1];
  gyro_yaw_input = JY901.stcGyro.w[2];
  
  angle_pitch  = JY901.stcAngle.Angle[0];
  angle_roll = JY901.stcAngle.Angle[1];
  angle_yaw = JY901.stcAngle.Angle[2];
}
