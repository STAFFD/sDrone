/*
 * The VIO algorithm requires IMU data which exceed 100Hz,
 * and the refresh rate of the flight control system is 250Hz.
 * So we send the IMU data every two loops and the the other
 * vital information in turns in the other loop.
 */

void telegram_send(){

  if(telegram_IMU){
    telegram_send_byte = 'I';
    tx_send();
    delayMicroseconds(15);

    tx_send_float(angle_pitch);
    tx_send_float(angle_roll);
    tx_send_float(angle_yaw);
    
    tx_send_float(gyro_pitch);
    tx_send_float(gyro_roll);
    tx_send_float(gyro_yaw);

    telegram_IMU = false;
  } else {

    tx_send_int8('R');
    tx_send_int8(13);
    tx_send_int8(142);
    tx_send_int8(battery_voltage * 10);
    tx_send_int8(timer);
    tx_send_int8(timer >> 8);
    telegram_IMU = true;
  }
}

void tx_send_int8(uint8_t data){
  telegram_send_byte = data;
  tx_send();
  delayMicroseconds(15);
}

void tx_send_float(float data){
  telegram_data_buffer = (data + 400) * 100;
  telegram_send_byte = telegram_data_buffer;
  tx_send();
  delayMicroseconds(15);
  telegram_send_byte = telegram_data_buffer >> 8;
  tx_send();
  delayMicroseconds(15);
}

void tx_send(){
  TELEGRAM_TX_START;                                                                       //Reset output PA9 to 0 to create a start bit.
  delayMicroseconds(17);                                                                   //Delay 17us (1s/5700bps)
  for (telemetry_bit_counter = 0; telemetry_bit_counter < 8; telemetry_bit_counter ++) {   //Create a loop fore every bit in the
    if (telegram_send_byte >> telemetry_bit_counter & 0b1) TELEGRAM_TX_HIGH;               //If the specific bit is set, set output PA9 to 1;
    else TELEGRAM_TX_LOW;                                                                  //If the specific bit is not set, reset output PA9 to 0;
    delayMicroseconds(17);                                                                 //Delay 17us (1s/57600bps)
  }
  //Send a stop bit
  TELEGRAM_TX_STOP;                                                                        //Set output PA9 to 1;
}
