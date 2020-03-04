///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//In this part the error LED signal is generated.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void error_signal(void){
  if (error >= 100) LED_RED(ON);                                            //When the error is 100 the LED is always on.
  else if (error_timer < millis()){                                       //If the error_timer value is smaller that the millis() function.
    error_timer = millis() + 250;                                         //Set the next error_timer interval at 250ms.
    if(error > 0 && error_counter > error + 3) error_counter = 0;         //If there is an error to report and the error_counter > error +3 reset the error.
    if (error_counter < error && error_led == 0 && error > 0){            //If the error flash sequence isn't finisched (error_counter < error) and the LED is off.
      LED_RED(ON);                                                          //Turn the LED on.
      error_led = 1;                                                      //Set the LED flag to indicate that the LED is on.
    }
    else{                                                                 //If the error flash sequence isn't finisched (error_counter < error) and the LED is on. 
      LED_RED(OFF);                                                         //Turn the LED off.
      error_counter++;                                                    //Increment the error_counter variable by 1 to keep trach of the flashes.
      error_led = 0;                                                      //Set the LED flag to indicate that the LED is off.
    }
  }
}

void load_flight_mode(){
  if(receiver_input[SWITCH_INPUT] > 950 && receiver_input[SWITCH_INPUT] < 1050) flightMode = MANUAL;
  else if(receiver_input[SWITCH_INPUT] > 1450 && receiver_input[SWITCH_INPUT] < 1550) flightMode = HEAD_LOCK;
  else if(receiver_input[SWITCH_INPUT] > 1950 && receiver_input[SWITCH_INPUT] < 2050) flightMode = VIO;
}

void loop_timer_init(){
  loop_timer = micros();                //Set the timer for the first loop.
}
/*
 * This function indicate that the setup process is finished.
 * flight_ready() should be place at the bottom of all setup process.
 */
void flight_ready(){
  //When everything is done, turn off the led.
  LED_RED(OFF);                                                 //Set output PB4 low.
  LED_GREEN(ON);
  loop_timer_init();
}
