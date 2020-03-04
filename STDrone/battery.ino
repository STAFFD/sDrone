//Load the battery voltage to the battery_voltage variable.
//The STM32 uses a 12 bit analog to digital converter.
//analogRead => 0 = 0V ..... 4095 = 3.3V
//The voltage divider (1k & 10k) is 1:11.
//analogRead => 0 = 0V ..... 4095 = 36.3V
//36.3 / 4095 = 112.81.
float read_Battery(){
  return battery_voltage * 0.92 + ((float)analogRead(BATTERY_PIN) / 1410.1);
}

void battery_init(){
  pinMode(BATTERY_PIN, INPUT_ANALOG);    //This is needed for reading the analog value of port A4.
  battery_voltage = (float)analogRead(BATTERY_PIN) / 112.81;
}
