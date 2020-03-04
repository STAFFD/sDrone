void esc_init(){
    //Arduino (Atmega) pins default to inputs, so they don't need to be explicitly declared as inputs.
  DDRB |= B01100000;                                            
  DDRC |= B01000000;  
  DDRD |= B10000000;  
}

void pull_esc(bool pull_direction){
  if(pull_direction){
    //Set digital outputs 6,7,8 and 9 high.
    PORTB |= B01100000;                                            
    PORTC |= B01000000;  
    PORTD |= B10000000;                                                   
  } else {
    PORTB &= B10011111;                                            
    PORTC &= B10111111;  
    PORTD &= B01111111;    
  }
}

void hold_esc(){
  for (cal_int = 0; cal_int < 500 ; cal_int ++){                           //Wait 2 seconds before continuing.
    pull_esc(ALL_UP);
    delayMicroseconds(1000);                                                //Wait 1000us.
    pull_esc(ALL_UP);
    delayMicroseconds(3000);                                                //Wait 3000us.
  }
}
