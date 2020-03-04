void debug_process(){

  #ifdef DEBUG_GYRO
    Serial.print("gyro_roll_input: ");
    Serial.print(gyro_roll_input);
    Serial.print(" gyro_pitch_input: ");
    Serial.println(gyro_pitch_input);
  #endif

  #ifdef DEBUG_BATTERY
    Serial.print("battery voltage: ");
    Serial.println(battery_voltage);
  #endif

  #ifdef DEBUG_ANGLE
    Serial.print("pitch_level_adjust: ");
    Serial.print(pitch_level_adjust);
    Serial.print(" roll_level_adjust: ");
    Serial.println(roll_level_adjust);
  #endif

  #ifdef DEBUG_RECEIVER
    Serial.print("receiver_input_channel_3(Throttle): ");
    Serial.print(receiver_input_channel_3);
    Serial.print(" receiver_input_channel_4(yaw): ");
    Serial.print(receiver_input_channel_4);
    Serial.print(" receiver_input_channel_1(roll): ");
    Serial.print(receiver_input_channel_1);
    Serial.print(" receiver_input_channel_2(pitch): ");
    Serial.println(receiver_input_channel_2);
  #endif
}
