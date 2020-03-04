void LED(bool on){
  on ? LED_ON : LED_OFF;
}

void BEEP(bool on){
  on ? BEEP_ON : BEEP_OFF;
}

void hints_init(){
  //Port PB3 and PB4 are used as JTDO and JNTRST by default.
  //The following function connects PB3 and PB4 to the
  //alternate output function.
  afio_cfg_debug_ports(AFIO_DEBUG_SW_ONLY);                    //Connects PB3 and PB4 to output function.
  GPIOB->regs->CRL = 0x11000; // Set the PB3 and PB4 to push-pull, 10MHZ, output mode
  BEEP(LOW);                                               //Set output PB3 low.
  LED(HIGH);                                                //Set output PB4 high.
}
