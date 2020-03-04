///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//In this part the error LED signal is generated.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void error_signal(void){
  if (error >= 100) LED(true);                                            //When the error is 100 the LED is always on.
  else if (error_timer < millis()){                                       //If the error_timer value is smaller that the millis() function.
    error_timer = millis() + 250;                                         //Set the next error_timer interval at 250ms.
    if(error > 0 && error_counter > error + 3) error_counter = 0;         //If there is an error to report and the error_counter > error +3 reset the error.
    if (error_counter < error && error_led == 0 && error > 0){            //If the error flash sequence isn't finisched (error_counter < error) and the LED is off.
      LED(true);                                                          //Turn the LED on.
      error_led = 1;                                                      //Set the LED flag to indicate that the LED is on.
    }
    else{                                                                 //If the error flash sequence isn't finisched (error_counter < error) and the LED is on. 
      LED(false);                                                         //Turn the LED off.
      error_counter++;                                                    //Increment the error_counter variable by 1 to keep trach of the flashes.
      error_led = 0;                                                      //Set the LED flag to indicate that the LED is off.
    }
  }
}

//Load the battery voltage to the battery_voltage variable.
//The STM32 uses a 12 bit analog to digital converter.
//analogRead => 0 = 0V ..... 4095 = 3.3V
//The voltage divider (1k & 10k) is 1:11.
//analogRead => 0 = 0V ..... 4095 = 36.3V
//36.3 / 4095 = 112.81.
float read_Battery(){
  return battery_voltage * 0.92 + ((float)analogRead(4) / 1410.1);
}

void battery_init(){
  battery_voltage = (float)analogRead(4) / 112.81;
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
  LED(LOW);                                                 //Set output PB4 low.
  BEEP(true);
  delay(100);
  BEEP(false);
  loop_timer_init();
}
