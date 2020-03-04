///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//In this part the various registers of the MPU-6050 are set.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void gyro_setup(void){
  Serial1.begin(115200);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//This part reads the raw gyro and accelerometer data from the GY901
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void gyro_signalen(void) {
  while (Serial1.available()) JY901.CopeSerialData(Serial1.read()); //Call JY901 data cope function
  gyro_pitch = gyroConvert(JY901.stcGyro.w[0], true);
  gyro_roll = gyroConvert(JY901.stcGyro.w[1], true);
  gyro_yaw = gyroConvert(JY901.stcGyro.w[2], true);

  lattitude = GPS_Convert(JY901.stcLonLat.lLat);
  longitude = GPS_Convert(JY901.stcLonLat.lLon);
  l_lat_gps = (int32_t)(lattitude * 1000000);
  l_lon_gps = (int32_t)(longitude * 1000000);
  
  angle_pitch  = angleConvert(JY901.stcAngle.Angle[0], ANGLE_PITCH_CALIBRATE_VALUE, true);
  angle_roll = angleConvert(JY901.stcAngle.Angle[1], ANGLE_ROLL_CALIBRATE_VALUE, true);
  angle_yaw = angleConvert(JY901.stcAngle.Angle[2], ANGLE_YAW_CALIBRATE_VALUE, true);
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
