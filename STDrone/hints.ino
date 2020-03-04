void hints_init(){
  //Port PB3 and PB4 are used as JTDO and JNTRST by default.
  //The following function connects PB3 and PB4 to the
  //alternate output function.
//  afio_cfg_debug_ports(AFIO_DEBUG_SW_ONLY);                    //Connects PB3 and PB4 to output function.
//  GPIOB->regs->CRL = 0x11000; // Set the PB3 and PB4 to push-pull, 10MHZ, output mode
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

[[deprecated]]
void BEEP(bool on){
  on ? BEEP_ON : BEEP_OFF;
}
