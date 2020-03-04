#define THROTTLE  3
#define YAW       0
#define PITCH     1
#define ROLL      2
#define TWIST     5
#define SWITCH    4

bool last_active_channel[6];
unsigned long current_time;
volatile int receiver_input[6], receive_channel_previous[6];
void setup(){
  Serial.begin(9600);
//  PCICR |= (1 << PCIE1);                                                    //Set PCIE1 to enable PCIE1 scan.
  PCICR |= (1 << PCIE0);                                                    //Set PCIE0 to enable PCMSK0 scan.
  PCMSK0 |= (1 << PCINT1);
  PCMSK0 |= (1 << PCINT2);                                                  //Set PCINT2 (digital input 10)to trigger an interrupt on state change.
  PCMSK0 |= (1 << PCINT3);                                                  //Set PCINT3 (digital input 11)to trigger an interrupt on state change.
  PCMSK0 |= (1 << PCINT4);                                                  //Set PCINT4 (digital input 11)to trigger an interrupt on state change.
//  PCMSK1 |= (1 << PCINT8);                                                  //Set PCINT8 (digital input 11)to trigger an interrupt on state change.
//  PCMSK1 |= (1 << PCINT9);                                                  //Set PCINT9 (digital input 11)to trigger an interrupt on state change.
//  PCMSK1 |= (1 << PCINT10);                                                 //Set PCINT10 (digital input 11)to trigger an interrupt on state change.
  pinMode(13, OUTPUT);
}

void loop(){
  Serial.print("THROTTLE: ");Serial.print(receiver_input[THROTTLE]);
  Serial.print(" YAW: ");Serial.print(receiver_input[YAW]);
  Serial.print(" PITCH: ");Serial.print(receiver_input[PITCH]);
  Serial.print(" ROLL: ");Serial.print(receiver_input[ROLL]);
  Serial.print(" TWIST: ");Serial.print(receiver_input[TWIST]);
  Serial.print(" SWITCH: ");Serial.println(receiver_input[SWITCH]);
  digitalWrite(13,HIGH);  
  delay(100);
  digitalWrite(13,LOW); 
  delay(100); 
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

//ISR(PCINT0_vect){
//  current_time = micros();
//
//  //Channel 1=========================================
//  if(PINB & 1 << 0){                                                     //Is input 8 high?
//    if(!last_active_channel[0]){                                                //Input 8 changed from 0 to 1.
//      last_active_channel[0] = true;                                                   //Remember current input state.
//      receive_channel_previous[0] = current_time;                                               //Set timer_1 to current_time.
//    }
//  }
//  else if(last_active_channel[0]){                                             //Input 8 is not high and changed from 1 to 0.
//    last_active_channel[0] = false;                                                     //Remember current input state.
//    receiver_input[0] = current_time - receive_channel_previous[0];                             //Channel 1 is current_time - timer_1.
//  }
//
//
//  //Channel 1=========================================
//  if(PINB & 1 << 2){                                                     //Is input 8 high?
//    if(!last_active_channel[1]){                                                //Input 8 changed from 0 to 1.
//      last_active_channel[1] = true;                                                   //Remember current input state.
//      receive_channel_previous[1] = current_time;                                               //Set timer_1 to current_time.
//    }
//  }
//  else if(last_active_channel[0]){                                             //Input 8 is not high and changed from 1 to 0.
//    last_active_channel[0] = false;                                                     //Remember current input state.
//    receiver_input[0] = current_time - receive_channel_previous[0];                             //Channel 1 is current_time - timer_1.
//  }
//
//  
//  //Channel 2=========================================
//  if(PINB & 1 << 3 ){                                                    //Is input 9 high?
//    if(!last_active_channel[4]){                                                //Input 9 changed from 0 to 1.
//      last_active_channel[4] = true;                                                   //Remember current input state.
//      receive_channel_previous[4] = current_time;                                               //Set timer_2 to current_time.
//    }
//  }
//  else if(last_active_channel[4]){                                             //Input 9 is not high and changed from 1 to 0.
//    last_active_channel[4] = false;                                                     //Remember current input state.
//    receiver_input[4] = current_time - receive_channel_previous[4];                             //Channel 2 is current_time - timer_2.
//  }
//  //Channel 3=========================================
//  if(PINB & 1 << 4 ){                                                    //Is input 10 high?
//    if(!last_active_channel[5]){                                                //Input 10 changed from 0 to 1.
//      last_active_channel[5] = true;                                                   //Remember current input state.
//      receive_channel_previous[5] = current_time;                                               //Set timer_3 to current_time.
//    }
//  }
//  else if(last_active_channel[5]){                                             //Input 10 is not high and changed from 1 to 0.
//    last_active_channel[5] = false;                                                     //Remember current input state.
//    receiver_input[5] = current_time - receive_channel_previous[5];                             //Channel 3 is current_time - timer_3.
//
//  }
//
//}

//ISR(PCINT1_vect){
//  current_time = micros();
//  //Channel 4=========================================
//  if(PINC & 1 << 0 ){                                                    //Is input 11 high?
//    if(!last_active_channel[3]){                                                //Input 11 changed from 0 to 1.
//      last_active_channel[3] = true;                                                   //Remember current input state.
//      receive_channel_previous[3] = current_time;                                               //Set timer_4 to current_time.
//    }
//  } else if(last_active_channel[3]){                                             //Input 11 is not high and changed from 1 to 0.
//    last_active_channel[3] = false;                                                     //Remember current input state.
//    receiver_input[3] = current_time - receive_channel_previous[3];                             //Channel 4 is current_time - timer_4.
//  }
//  //Channel 5=========================================
//  if(PINC & 1 << 1 ){                                                    //Is input 11 high?
//    if(!last_active_channel[1]){                                                //Input 11 changed from 0 to 1.
//      last_active_channel[1] = true;                                                   //Remember current input state.
//      receive_channel_previous[1] = current_time;                                               //Set timer_4 to current_time.
//    }
//  } else if(last_active_channel[1]){                                             //Input 11 is not high and changed from 1 to 0.
//    last_active_channel[1] = false;                                                     //Remember current input state.
//    receiver_input[1] = current_time - receive_channel_previous[1];                             //Channel 4 is current_time - timer_4.
//  }
//    //Channel 6=========================================
//  if(PINC & 1 << 2 ){                                                    //Is input 11 high?
//    if(!last_active_channel[2]){                                                //Input 11 changed from 0 to 1.
//      last_active_channel[2] = true;                                                   //Remember current input state.
//      receive_channel_previous[2] = current_time;                                               //Set timer_4 to current_time.
//    }
//  }
//  else if(last_active_channel[2]){                                             //Input 11 is not high and changed from 1 to 0.
//    last_active_channel[2] = false;                                                     //Remember current input state.
//    receiver_input[2] = current_time - receive_channel_previous[2];                             //Channel 4 is current_time - timer_4.
//  }
//}
