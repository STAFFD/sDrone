void reading_receiver_signals(void) {
  while (data != 'q') {                                                                   //Stay in this loop until the data variable data holds a q.
    delay(250);                                                                           //Print the receiver values on the screen every 250ms
    if (Serial.available() > 0) {                                                         //If serial data is available
      data = Serial.read();                                                               //Read the incomming byte
      delay(100);                                                                         //Wait for any other bytes to come in
      while (Serial.available() > 0)loop_counter = Serial.read();                         //Empty the Serial buffer
    }
    //For starting the motors: throttle low and yaw left (step 1).
    if (receiver_input[THROTTLE_INPUT] < 1100 && receiver_input[YAW_INPUT] < 1100)start = 1;
    //When yaw stick is back in the center position start the motors (step 2).
    if (start == 1 && receiver_input[THROTTLE_INPUT] < 1100 && receiver_input[YAW_INPUT] > 1450)start = 2;
    //Stopping the motors: throttle low and yaw right.
    if (start == 2 && receiver_input[THROTTLE_INPUT] < 1100 && receiver_input[YAW_INPUT] > 1900)start = 0;

    Serial.print("Start:");
    Serial.print(start);

    Serial.print("  Roll:");
    if (receiver_input[ROLL_INPUT] - 1480 < 0)Serial.print("<<<");
    else if (receiver_input[ROLL_INPUT] - 1520 > 0)Serial.print(">>>");
    else Serial.print("-+-");
    Serial.print(receiver_input[ROLL_INPUT]);

    Serial.print("  Pitch:");
    if (receiver_input[PITCH_INPUT] - 1480 < 0)Serial.print("^^^");
    else if (receiver_input[PITCH_INPUT] - 1520 > 0)Serial.print("vvv");
    else Serial.print("-+-");
    Serial.print(receiver_input[PITCH_INPUT]);

    Serial.print("  Throttle:");
    if (receiver_input[THROTTLE_INPUT] - 1480 < 0)Serial.print("vvv");
    else if (receiver_input[THROTTLE_INPUT] - 1520 > 0)Serial.print("^^^");
    else Serial.print("-+-");
    Serial.print(receiver_input[THROTTLE_INPUT]);

    Serial.print("  Yaw:");
    if (receiver_input[YAW_INPUT] - 1480 < 0)Serial.print("<<<");
    else if (receiver_input[YAW_INPUT] - 1520 > 0)Serial.print(">>>");
    else Serial.print("-+-");
    Serial.print(receiver_input[YAW_INPUT]);

    Serial.print("  twist:");
    Serial.print(receiver_input[TWIST_INPUT]);

    Serial.print("  switch:");
    Serial.println(receiver_input[SWITCH_INPUT]);

  }
  print_intro();
}
