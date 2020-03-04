void debug_process(){

  #ifdef DEBUG_GYRO
    Serial.print("gyro_roll: ");
    Serial.print(gyro_roll);
    Serial.print(" gyro_pitch: ");
    Serial.print(gyro_pitch);
    Serial.print(" gyro_yaw: ");
    Serial.println(gyro_yaw);
  #endif

  #ifdef DEBUG_BLUETOOTH
    Serial.print(lattitude);
    Serial.print(" ");
    Serial.println(longitude);
  #endif

  #ifdef DEBUG_GPS
    Serial.print("lattitude: ");
    Serial.print(lattitude, 15);
    Serial.print(" longitude: ");
    Serial.print(longitude, 15);
    Serial.println(" ");
  #endif
  
  #ifdef DEBUG_CAMPASS_RAW
    Serial.print("campassX: ");
    Serial.print(campassX);
    Serial.print(" campassY: ");
    Serial.print(campassY);
    Serial.print(" campassZ: ");
    Serial.println(campassZ);
  #endif

  #ifdef DEBUG_CAMPASS
    Serial.print(" campass: ");
    Serial.println(angle_yaw);
  #endif

  #ifdef DEBUG_ANGLE
    Serial.print("angle_pitch: ");
    Serial.print(angle_pitch);
    Serial.print(" angle_roll: ");
    Serial.print(angle_roll);
    Serial.print(" angle_yaw: ");
    Serial.println(angle_yaw);
  #endif

  #ifdef DEBUG_LOOP_TIME
    Serial.println(timer);
  #endif

  #ifdef DEBUG_BATTERY
    Serial.print(" battery: ");
    Serial.println(battery_voltage);
  #endif
  
  #ifdef DEBUG_RECEIVER
    Serial.print("THROTTLE_INPUT: ");
    Serial.print(receiver_input[THROTTLE_INPUT]);
    Serial.print(" YAW_INPUT: ");
    Serial.print(receiver_input[YAW_INPUT]);
    Serial.print(" PITCH_INPUT: ");
    Serial.print(receiver_input[PITCH_INPUT]);
    Serial.print(" ROLL_INPUT: ");
    Serial.println(receiver_input[ROLL_INPUT]);
  #endif
}
