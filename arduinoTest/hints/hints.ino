#define ON  true
#define OFF false
void hints_init(){
  DDRB |= B00100000;              //Configure digital poort 13 as output.
  DDRC |= B00001000;              //Configure digital poort A6 as output.
}

void LED(bool on){
  if(on) PORTB |= B00100000;  
  else PORTB &= B11011111;  
}

void LED_toggle(){
  if(PINB & B00100000 ) LED(OFF);
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

void setup(){
    hints_init();
}

void loop(){
  delay(100);
  LED_toggle();
  BEEP_toggle();
  delay(100);
}
