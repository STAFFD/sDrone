void receiver_init(){
  PCICR  |= (1 << PCIE0);                                                    //Set PCIE0 to enable PCMSK0 scan.
  PCMSK0 |= (1 << PCINT1);                                                  //Set PCINT2 (digital input 10)to trigger an interrupt on state change.
  PCMSK0 |= (1 << PCINT2);                                                  //Set PCINT2 (digital input 10)to trigger an interrupt on state change.
  PCMSK0 |= (1 << PCINT3);                                                  //Set PCINT3 (digital input 11)to trigger an interrupt on state change.
  PCMSK0 |= (1 << PCINT4);                                                  //Set PCINT4 (digital input 11)to trigger an interrupt on state change.
}

void receiver_check(){
  //Wait until the receiver is active and the throtle is set to the lower position.
  while(receiver_input_channel_3 < 990 || receiver_input_channel_3 > 1020 || receiver_input_channel_4 < 1400){
    receiver_input_channel_3 = convert_receiver_channel(3);                 //Convert the actual receiver signals for throttle to the standard 1000 - 2000us
    receiver_input_channel_4 = convert_receiver_channel(4);                 //Convert the actual receiver signals for yaw to the standard 1000 - 2000us
    start ++;                                                               //While waiting increment start whith every loop.
    //Wait until the receiver is active and the throtle is set to the lower position.
    pull_esc(ALL_UP);
    delayMicroseconds(1000);                                                //Wait 1000us.
    pull_esc(ALL_DOWN);
    if(start == 125){                                                       //Every 125 loops (500ms).
      LED_toggle();                                   //Change the led status.
      start = 0;                                                            //Start again at 0.
    }
  }
  start = 0;                                                                //Set start back to 0.  
}

//This part converts the actual receiver signals to a standardized 1000 – 1500 – 2000 microsecond value.
//The stored data in the EEPROM is used.
int convert_receiver_channel(byte function){
  byte channel, reverse;                                                       //First we declare some local variables
  int low, center, high, actual;
  int difference;

  channel = eeprom_data[function + 23] & 0b00000111;                           //What channel corresponds with the specific function
  if(eeprom_data[function + 23] & 0b10000000)reverse = 1;                      //Reverse channel when most significant bit is set
  else reverse = 0;                                                            //If the most significant is not set there is no reverse

  actual = receiver_input[channel-1];                                            //Read the actual receiver value for the corresponding function
  low = (eeprom_data[channel * 2 + 15] << 8) | eeprom_data[channel * 2 + 14];  //Store the low value for the specific receiver input channel
  center = (eeprom_data[channel * 2 - 1] << 8) | eeprom_data[channel * 2 - 2]; //Store the center value for the specific receiver input channel
  high = (eeprom_data[channel * 2 + 7] << 8) | eeprom_data[channel * 2 + 6];   //Store the high value for the specific receiver input channel

  if(actual < center){                                                         //The actual receiver value is lower than the center value
    if(actual < low)actual = low;                                              //Limit the lowest value to the value that was detected during setup
    difference = ((long)(center - actual) * (long)500) / (center - low);       //Calculate and scale the actual value to a 1000 - 2000us value
    if(reverse == 1)return 1500 + difference;                                  //If the channel is reversed
    else return 1500 - difference;                                             //If the channel is not reversed
  }
  else if(actual > center){                                                                        //The actual receiver value is higher than the center value
    if(actual > high)actual = high;                                            //Limit the lowest value to the value that was detected during setup
    difference = ((long)(actual - center) * (long)500) / (high - center);      //Calculate and scale the actual value to a 1000 - 2000us value
    if(reverse == 1)return 1500 - difference;                                  //If the channel is reversed
    else return 1500 + difference;                                             //If the channel is not reversed
  }
  else return 1500;
}

ISR(PCINT0_vect){
  current_time = micros();
  //Channel 1=========================================
  if(PINB & 1 << 4){                                                     //Is input 8 high?
    if(!last_active_channel[0]){                                                //Input 8 changed from 0 to 1.
      last_active_channel[0] = true;                                                   //Remember current input state.
      receive_channel_previous[0] = current_time;                                               //Set timer_1 to current_time.
    }
  }
  else if(last_active_channel[0]){                                             //Input 8 is not high and changed from 1 to 0.
    last_active_channel[0] = false;                                                     //Remember current input state.
    receiver_input[0] = current_time - receive_channel_previous[0];                             //Channel 1 is current_time - timer_1.
  }

  
  //Channel 2=========================================
  if(PINB & 1 << 1 ){                                                    //Is input 9 high?
    if(!last_active_channel[1]){                                                //Input 9 changed from 0 to 1.
      last_active_channel[1] = true;                                                   //Remember current input state.
      receive_channel_previous[1] = current_time;                                               //Set timer_2 to current_time.
    }
  }
  else if(last_active_channel[1]){                                             //Input 9 is not high and changed from 1 to 0.
    last_active_channel[1] = false;                                                     //Remember current input state.
    receiver_input[1] = current_time - receive_channel_previous[1];                             //Channel 2 is current_time - timer_2.
  }
  //Channel 3=========================================
  if(PINB & 1 << 2 ){                                                    //Is input 10 high?
    if(!last_active_channel[2]){                                                //Input 10 changed from 0 to 1.
      last_active_channel[2] = true;                                                   //Remember current input state.
      receive_channel_previous[2] = current_time;                                               //Set timer_3 to current_time.
    }
  }
  else if(last_active_channel[2]){                                             //Input 10 is not high and changed from 1 to 0.
    last_active_channel[2] = false;                                                     //Remember current input state.
    receiver_input[2] = current_time - receive_channel_previous[2];                             //Channel 3 is current_time - timer_3.

  }

  //Channel 3=========================================
  if(PINB & 1 << 3 ){                                                    //Is input 10 high?
    if(!last_active_channel[3]){                                                //Input 10 changed from 0 to 1.
      last_active_channel[3] = true;                                                   //Remember current input state.
      receive_channel_previous[3] = current_time;                                               //Set timer_3 to current_time.
    }
  }
  else if(last_active_channel[3]){                                             //Input 10 is not high and changed from 1 to 0.
    last_active_channel[3] = false;                                                     //Remember current input state.
    receiver_input[3] = current_time - receive_channel_previous[3];                             //Channel 3 is current_time - timer_3.

  }
}
