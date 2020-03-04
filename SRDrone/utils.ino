#include "SRDrone.h"
void timer_init(){
  loop_timer = micros();
}

void hints_init(){
  DDRD |= B00010000;              //Configure digital poort 13 as output.
}

void LED(bool on){
  if(on) PORTD|= B00010000;  
  else PORTD &= B00100000;  
}

void LED_toggle(){
  if(PIND & B00010000 ) LED(OFF);
  else LED(ON);
}

void BEEP(bool on){
  if(on) PORTC |= B00001000;  
  else PORTC &= B11110111;  
}

void BEEP_toggle(){
  if(PINC & B00001000 ) BEEP(OFF);
  else BEEP(ON);
}

void battery_init(){
  //Load the battery voltage to the battery_voltage variable.
  //65 is the voltage compensation for the diode.
  //12.6V equals ~5V @ Analog 0.
  //12.6V equals 1023 analogRead(0).
  //1260 / 1023 = 1.2317.
  //The variable battery_voltage holds 1050 if the battery voltage is 10.5V.
  //battery_voltage = (analogRead(0) + 65) * 1.2317;
  // Now we do not need the diode because the connector is one way only.
  battery_voltage = (analogRead(BATTERY_PIN)) * 1.2317;
}

float readBattery(){
  return battery_voltage * 0.92 + (analogRead(BATTERY_PIN)) * 0.09853;  
}

void battery_alarm(){
  if(battery_time < micros()){
    battery_time = micros() + 100000;
    BEEP_toggle();
    LED_toggle();
  }
}

void calculate_battery(){
  //The battery voltage is needed for compensation.
  //A complementary filter is used to reduce noise.
  //0.09853 = 0.08 * 1.2317.
  battery_voltage = readBattery();
  //Turn on the led if battery voltage is too low.
  if(battery_voltage < 1000 && battery_voltage > 600) battery_alarm();
}
