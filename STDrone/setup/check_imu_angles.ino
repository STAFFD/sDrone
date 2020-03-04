void check_imu_angles(void) {
  uint8_t first_angle = 0;
  loop_counter = 0;
  first_angle = false;
  if (use_manual_calibration)cal_int = 2000;                                            //If manual calibration is used.
  else {
    cal_int = 0;                                                                        //If manual calibration is not used.
    manual_gyro_pitch_cal_value = 0;                                                    //Set the manual pitch calibration variable to 0.
    manual_gyro_roll_cal_value = 0;                                                     //Set the manual roll calibration variable to 0.
    manual_gyro_yaw_cal_value = 0;                                                      //Set the manual yaw calibration variable to 0.
  }
  while (data != 'q') {                                                                 //Stay in this loop until the data variable data holds a q.
    loop_timer = micros() + 4000;                                                       //Set the loop_timer variable to the current micros() value + 4000.
    if (Serial.available() > 0) {                                                       //If serial data is available.
      data = Serial.read();                                                             //Read the incomming byte.
      delay(100);                                                                       //Wait for any other bytes to come in.
      while (Serial.available() > 0)loop_counter = Serial.read();                       //Empty the Serial buffer.
    }

    gyro_signalen();                                                                    //Let's get the current gyro data.

    angle_pitch = acc_axis[1];
    angle_roll = acc_axis[2];

    //We can't print all the data at once. This takes to long and the angular readings will be off.
    if (loop_counter == 0)Serial.print("Pitch: ");
    if (loop_counter == 1)Serial.print(angle_pitch , 1);
    if (loop_counter == 2)Serial.print(" Roll: ");
    if (loop_counter == 3)Serial.print(angle_roll , 1);
    if (loop_counter == 4)Serial.print(" Yaw: ");
    if (loop_counter == 5)Serial.print(gyro_axis[3], 1);
    if (loop_counter == 6)Serial.print(" Temp: ");
    if (loop_counter == 7)Serial.println(temperature , 1);
    loop_counter ++;
    if (loop_counter == 60)loop_counter = 0;

    while (loop_timer > micros());
  }
  loop_counter = 0;                                                                     //Reset the loop counter variable to 0.
  print_intro();                                                                        //Print the intro to the serial monitor.
}
