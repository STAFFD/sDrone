///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Setup routine
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void setup() {
  #ifdef DEBUG
    Serial.begin(57600);
  #else
    pinMode(PA9, OUTPUT);
  #endif
  hints_init();
  receiver_init();                                              //Setup the timers for the receiver output.
  esc_init();                                                   //Setup the timers for the ESC output.
  delay(50);                                                    //Give the timers some time to start.
  gyro_setup();                                                 //Initiallize the gyro and set the correct registers.
  battery_init();
  #ifdef DEBUG_HARDWARE
    while(true){
      gyro_signalen();
      battery_voltage = read_Battery();
      debug_process();
      delay(200);
    }
  #endif
  receiver_check();
  flight_ready();
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Main program loop
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void loop() {

  #ifdef DEBUG
    debug_process();
  #endif

  telegram_send();
  load_flight_mode();
  error_signal();                                                                  //Show the errors via the red LED.
  gyro_signalen();                                                                 //Read the gyro and accelerometer data.

  pitch_level_adjust = angle_pitch * 15;                                           //Calculate the pitch angle correction.
  roll_level_adjust = angle_roll * 15;                                             //Calculate the roll angle correction.

  //For starting the motors: throttle low and yaw left (step 1).
  if (receiver_input[THROTTLE_INPUT] < 1050 && receiver_input[YAW_INPUT] < 1050)start = 1;
  //When yaw stick is back in the center position start the motors (step 2).
  if (start == 1 && receiver_input[THROTTLE_INPUT] < 1050 && receiver_input[YAW_INPUT] > 1450) {
    start = 2;

    LED_GREEN(OFF);                                                                //Turn off the green led.

    //Reset the PID controllers for a bumpless start.
    pid_i_mem_roll = 0;
    pid_last_roll_d_error = 0;
    pid_i_mem_pitch = 0;
    pid_last_pitch_d_error = 0;
    pid_i_mem_yaw = 0;
    pid_last_yaw_d_error = 0;
  }
  //Stopping the motors: throttle low and yaw right.
  if (start == 2 && receiver_input[THROTTLE_INPUT] < 1050 && receiver_input[YAW_INPUT] > 1950) {
    start = 0;
    LED_GREEN(ON);                                                               //Turn on the green led.
  }

  //The PID set point in degrees per second is determined by the roll receiver input.
  //In the case of deviding by 3 the max roll rate is aprox 164 degrees per second ( (500-8)/3 = 164d/s ).
  
  //We need a little dead band of 16us for better results.
  if (receiver_input[ROLL_INPUT] > 1508)pid_roll_setpoint = receiver_input[ROLL_INPUT] - 1508;
  else if (receiver_input[ROLL_INPUT] < 1492)pid_roll_setpoint = receiver_input[ROLL_INPUT] - 1492;
  else pid_roll_setpoint = 0;
  
  pid_roll_setpoint -= roll_level_adjust;                                          //Subtract the angle correction from the standardized receiver roll input value.
  pid_roll_setpoint /= 3.0;                                                        //Divide the setpoint for the PID roll controller by 3 to get angles in degrees.


  //The PID set point in degrees per second is determined by the pitch receiver input.
  //In the case of deviding by 3 the max pitch rate is aprox 164 degrees per second ( (500-8)/3 = 164d/s ).
  
  //We need a little dead band of 16us for better results.
  if (receiver_input[PITCH_INPUT] > 1508)pid_pitch_setpoint = receiver_input[PITCH_INPUT] - 1508;
  else if (receiver_input[PITCH_INPUT] < 1492)pid_pitch_setpoint = receiver_input[PITCH_INPUT] - 1492;
  else pid_pitch_setpoint = 0;
  
  pid_pitch_setpoint -= pitch_level_adjust;                                        //Subtract the angle correction from the standardized receiver pitch input value.
  pid_pitch_setpoint /= 3.0;                                                       //Divide the setpoint for the PID pitch controller by 3 to get angles in degrees.

  //The PID set point in degrees per second is determined by the yaw receiver input.
  //In the case of deviding by 3 the max yaw rate is aprox 164 degrees per second ( (500-8)/3 = 164d/s ).
  pid_yaw_setpoint = 0;
  //We need a little dead band of 16us for better results.
  if (receiver_input[THROTTLE_INPUT] > 1050) { //Do not yaw when turning off the motors.
    if (receiver_input[YAW_INPUT] > 1508)pid_yaw_setpoint = (receiver_input[YAW_INPUT] - 1508) / 3.0;
    else if (receiver_input[YAW_INPUT] < 1492)pid_yaw_setpoint = (receiver_input[YAW_INPUT] - 1492) / 3.0;
  }

  calculate_pid();                                                                 //PID inputs are known. So we can calculate the pid output.

  //The battery voltage is needed for compensation.
  //A complementary filter is used to reduce noise.
  //1410.1 = 112.81 / 0.08.
  battery_voltage = read_Battery();

  //Turn on the led if battery voltage is to low. In this case under 10.0V
  if (battery_voltage < 10.0 && error == 0)error = 1;
  
  throttle = receiver_input[THROTTLE_INPUT];                                                            //We need the throttle signal as a base signal.

  if (start == 2) {                                                                //The motors are started.
    if (throttle > 1800) throttle = 1800;                                          //We need some room to keep full control at full throttle.
    esc_1 = throttle - pid_output_pitch + pid_output_roll - pid_output_yaw - pid_output_head;        //Calculate the pulse for esc 1 (front-right - CCW).
    esc_2 = throttle + pid_output_pitch + pid_output_roll + pid_output_yaw + pid_output_head;        //Calculate the pulse for esc 2 (rear-right - CW).
    esc_3 = throttle + pid_output_pitch - pid_output_roll - pid_output_yaw - pid_output_head;        //Calculate the pulse for esc 3 (rear-left - CCW).
    esc_4 = throttle - pid_output_pitch - pid_output_roll + pid_output_yaw + pid_output_head;        //Calculate the pulse for esc 4 (front-left - CW).

    if (esc_1 < MOTOR_MIN) esc_1 = MOTOR_MIN;                                                //Keep the motors running.
    if (esc_2 < MOTOR_MIN) esc_2 = MOTOR_MIN;                                                //Keep the motors running.
    if (esc_3 < MOTOR_MIN) esc_3 = MOTOR_MIN;                                                //Keep the motors running.
    if (esc_4 < MOTOR_MIN) esc_4 = MOTOR_MIN;                                                //Keep the motors running.

    if (esc_1 > MOTOR_MAX)esc_1 = MOTOR_MAX;                                                 //Limit the esc-1 pulse to 2000us.
    if (esc_2 > MOTOR_MAX)esc_2 = MOTOR_MAX;                                                 //Limit the esc-2 pulse to 2000us.
    if (esc_3 > MOTOR_MAX)esc_3 = MOTOR_MAX;                                                 //Limit the esc-3 pulse to 2000us.
    if (esc_4 > MOTOR_MAX)esc_4 = MOTOR_MAX;                                                 //Limit the esc-4 pulse to 2000us.
  }

  else {
    esc_1 = MOTOR_STOP;                                                                  //If start is not 2 keep a 1000us pulse for ess-1.
    esc_2 = MOTOR_STOP;                                                                  //If start is not 2 keep a 1000us pulse for ess-2.
    esc_3 = MOTOR_STOP;                                                                  //If start is not 2 keep a 1000us pulse for ess-3.
    esc_4 = MOTOR_STOP;                                                                  //If start is not 2 keep a 1000us pulse for ess-4.
  }

  TIMER4_BASE->CCR1 = esc_1;                                                       //Set the throttle receiver input pulse to the ESC 1 output pulse.
  TIMER4_BASE->CCR2 = esc_2;                                                       //Set the throttle receiver input pulse to the ESC 2 output pulse.
  TIMER4_BASE->CCR3 = esc_3;                                                       //Set the throttle receiver input pulse to the ESC 3 output pulse.
  TIMER4_BASE->CCR4 = esc_4;                                                       //Set the throttle receiver input pulse to the ESC 4 output pulse.
  TIMER4_BASE->CNT = 5000;                                                         //This will reset timer 4 and the ESC pulses are directly created.

  //! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! !
  //Because of the angle calculation the loop time is getting very important. If the loop time is
  //longer or shorter than 4000us the angle calculation is off. If you modify the code make sure
  //that the loop time is still 4000us and no longer! More information can be found on
  //! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! !

  timer = micros() - loop_timer;
  if (micros() - loop_timer > LOOP_TIME_MAX)error = 5;                                      //Turn on the LED if the loop time exceeds 4050us.
  while (micros() - loop_timer < LOOP_TIME_MIN);                                            //We wait until 4000us are passed.
  loop_timer = micros();                                                           //Set the timer for the next loop.
}
