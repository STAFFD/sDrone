///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Subroutine for calculating pid outputs
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void calculate_pid(void){

  roll_PID_Calculation();
  pitch_PID_Calculation();
  
  
  if(flightMode == HEAD_LOCK) headLock_PID_Calculation();
  else {
    pid_last_head_d_error = 0;
    pid_i_mem_head = 0;
    pid_output_head = 0;
    yaw_PID_Calculation();
  }
}

void roll_PID_Calculation(){
  //Roll calculations
  pid_error_temp = gyro_roll - pid_roll_setpoint;
  pid_i_mem_roll += pid_i_gain_roll * pid_error_temp;
  if(pid_i_mem_roll > pid_max_roll)pid_i_mem_roll = pid_max_roll;
  else if(pid_i_mem_roll < pid_max_roll * -1)pid_i_mem_roll = pid_max_roll * -1;

  pid_output_roll = pid_p_gain_roll * pid_error_temp + pid_i_mem_roll + pid_d_gain_roll * (pid_error_temp - pid_last_roll_d_error);
  if(pid_output_roll > pid_max_roll)pid_output_roll = pid_max_roll;
  else if(pid_output_roll < pid_max_roll * -1)pid_output_roll = pid_max_roll * -1;

  pid_last_roll_d_error = pid_error_temp;
}

void pitch_PID_Calculation(){
  //Pitch calculations
  pid_error_temp = gyro_pitch - pid_pitch_setpoint;
  pid_i_mem_pitch += pid_i_gain_pitch * pid_error_temp;
  if(pid_i_mem_pitch > pid_max_pitch)pid_i_mem_pitch = pid_max_pitch;
  else if(pid_i_mem_pitch < pid_max_pitch * -1)pid_i_mem_pitch = pid_max_pitch * -1;

  pid_output_pitch = pid_p_gain_pitch * pid_error_temp + pid_i_mem_pitch + pid_d_gain_pitch * (pid_error_temp - pid_last_pitch_d_error);
  if(pid_output_pitch > pid_max_pitch)pid_output_pitch = pid_max_pitch;
  else if(pid_output_pitch < pid_max_pitch * -1)pid_output_pitch = pid_max_pitch * -1;

  pid_last_pitch_d_error = pid_error_temp;
}

void yaw_PID_Calculation(){
  //Yaw calculations
  pid_error_temp = gyro_yaw - pid_yaw_setpoint;
  pid_i_mem_yaw += pid_i_gain_yaw * pid_error_temp;
  if(pid_i_mem_yaw > pid_max_yaw)pid_i_mem_yaw = pid_max_yaw;
  else if(pid_i_mem_yaw < pid_max_yaw * -1)pid_i_mem_yaw = pid_max_yaw * -1;

  pid_output_yaw = pid_p_gain_yaw * pid_error_temp + pid_i_mem_yaw + pid_d_gain_yaw * (pid_error_temp - pid_last_yaw_d_error);
  if(pid_output_yaw > pid_max_yaw)pid_output_yaw = pid_max_yaw;
  else if(pid_output_yaw < pid_max_yaw * -1)pid_output_yaw = pid_max_yaw * -1;

  pid_last_yaw_d_error = pid_error_temp;
}

void headLock_PID_Calculation(){
  pid_error_temp = angle_yaw - pid_head_setpoint;
  pid_i_mem_head += pid_i_gain_head * pid_error_temp;
  if(pid_i_mem_head > pid_max_head) pid_i_mem_head = pid_max_head;
  else if(pid_i_mem_head > pid_max_head * -1) pid_i_mem_head = pid_max_head * -1;

  pid_output_head = pid_p_gain_head * pid_error_temp + pid_i_mem_head + pid_d_gain_head * (pid_error_temp - pid_last_head_d_error);
  if(pid_output_head > pid_max_head) pid_output_head = pid_max_head;
  else if(pid_output_head < pid_max_head * -1) pid_output_head = pid_max_head * -1;

  pid_last_head_d_error = pid_error_temp;
}
