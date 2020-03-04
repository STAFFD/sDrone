#include <Wire.h>

#define gyro_address 0x68
float acc_y, acc_x, acc_z, acc_total_vector;
float angle_pitch_acc, angle_roll_acc;
float gyro_roll, gyro_pitch, gyro_yaw;
float temperature;
TwoWire MPUWIRE(2);  

void setup() {
  Serial.begin(57600);
  delay(250);
  Serial.println("fuck");

//  MPUWIRE.setClock(400000);
  MPUWIRE.begin();                                                //Start the I2C as master
  delay(250);
  
  MPUWIRE.beginTransmission(gyro_address);                        //Start communication with the MPU-6050.
  MPUWIRE.write(0x6B);                                            //We want to write to the PWR_MGMT_1 register (6B hex).
  MPUWIRE.write(0x00);                                            //Set the register bits as 00000000 to activate the gyro.
  MPUWIRE.endTransmission();                                      //End the transmission with the gyro.

  MPUWIRE.beginTransmission(gyro_address);                        //Start communication with the MPU-6050.
  MPUWIRE.write(0x1B);                                            //We want to write to the GYRO_CONFIG register (1B hex).
  MPUWIRE.write(0x08);                                            //Set the register bits as 00001000 (500dps full scale).
  MPUWIRE.endTransmission();                                      //End the transmission with the gyro.

  MPUWIRE.beginTransmission(gyro_address);                        //Start communication with the MPU-6050.
  MPUWIRE.write(0x1C);                                            //We want to write to the ACCEL_CONFIG register (1A hex).
  MPUWIRE.write(0x10);                                            //Set the register bits as 00010000 (+/- 8g full scale range).
  MPUWIRE.endTransmission();                                      //End the transmission with the gyro.

  MPUWIRE.beginTransmission(gyro_address);                        //Start communication with the MPU-6050.
  MPUWIRE.write(0x1A);                                            //We want to write to the CONFIG register (1A hex).
  MPUWIRE.write(0x03);                                            //Set the register bits as 00000011 (Set Digital Low Pass Filter to ~43Hz).
  MPUWIRE.endTransmission();                                      //End the transmission with the gyro.
}

// the loop function runs over and over again forever
void loop() {

  MPUWIRE.beginTransmission(gyro_address);
  MPUWIRE.write(0x3B);
  MPUWIRE.endTransmission();
  MPUWIRE.requestFrom(gyro_address, 14);

  while(MPUWIRE.available() < 14); 
  acc_y = MPUWIRE.read() << 8 | MPUWIRE.read();                    //Add the low and high byte to the acc_x variable.
  acc_x = MPUWIRE.read() << 8 | MPUWIRE.read();                    //Add the low and high byte to the acc_y variable.
  acc_z = MPUWIRE.read() << 8 | MPUWIRE.read();                    //Add the low and high byte to the acc_z variable.
  temperature = MPUWIRE.read() << 8 | MPUWIRE.read();              //Add the low and high byte to the temperature variable.
  gyro_roll = MPUWIRE.read() << 8 | MPUWIRE.read();                //Read high and low part of the angular data.
  gyro_pitch = MPUWIRE.read() << 8 | MPUWIRE.read();               //Read high and low part of the angular data.
  gyro_yaw = MPUWIRE.read() << 8 | MPUWIRE.read();                 //Read high and low part of the angular data.

  //Accelerometer angle calculations
  acc_total_vector = sqrt((acc_x * acc_x) + (acc_y * acc_y) + (acc_z * acc_z));    //Calculate the total accelerometer vector.

  if (abs(acc_y) < acc_total_vector) {                                             //Prevent the asin function to produce a NaN.
    angle_pitch_acc = asin((float)acc_y / acc_total_vector) * 57.296;              //Calculate the pitch angle.
  }
  if (abs(acc_x) < acc_total_vector) {                                             //Prevent the asin function to produce a NaN.
    angle_roll_acc = asin((float)acc_x / acc_total_vector) * 57.296;               //Calculate the roll angle.
  }

  Serial.print("angle_pitch_acc = ");
  Serial.print((float)angle_pitch_acc);
  Serial.print(" angle_roll_acc = ");
  Serial.print((float)angle_roll_acc);

  Serial.print(" Gx = ");
  Serial.print((float)gyro_roll/ 65.5);
  Serial.print(" Gy = ");
  Serial.print((float)gyro_pitch/ 65.5);
  Serial.print(" Gz = ");
  Serial.print((float)gyro_yaw/ 65.5);

  Serial.print(" T = ");
  Serial.println(temperature);
  delay(10);
}
