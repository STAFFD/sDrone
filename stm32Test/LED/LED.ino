#define LED_RED_ON              GPIOC->regs->ODR |= 1 << 14
#define LED_RED_OFF             GPIOC->regs->ODR &= ~(1 << 14)
#define LED_RED_CHANGE_STATE()  (GPIOC->regs->ODR & 1 << 14) ? LED_OFF : LED_ON

#define LED_GREEN_ON             GPIOC->regs->ODR |= 1 << 15
#define LED_GREEN_OFF            GPIOC->regs->ODR &= ~(1 << 15)
#define LED_GREEN_CHANGE_STATE() (GPIOC->regs->ODR & 1 << 15) ? BEEP_OFF : BEEP_ON

#define ON true
#define OFF false

void hints_init(){
  //Port PB3 and PB4 are used as JTDO and JNTRST by default.
  //The following function connects PB3 and PB4 to the
  //alternate output function.
//  afio_cfg_debug_ports(AFIO_DEBUG_SW_ONLY);                    //Connects PB3 and PB4 to output function.
  GPIOB->regs->CRL = 0x11000; // Set the PB3 and PB4 to push-pull, 10MHZ, output mode
  GPIOC->regs->CRH |= 0x11100000; // Set the PC13 to push-pull, 10MHZ, output mode
  LED_GREEN(OFF);                                               //Set output PB3 low.
  LED_RED(ON);                                                //Set output PB4 high.
}

void LED_RED(bool on){
  on ? LED_RED_ON : LED_RED_OFF;
}

void LED_GREEN(bool on){
  on ? LED_GREEN_ON : LED_GREEN_OFF;
}
void setup(){
  hints_init();
}

void loop(){

  LED_RED(true);
  LED_GREEN(false);
  delay(500);
  LED_RED(false);
  LED_GREEN(true);
  delay(500);
}
