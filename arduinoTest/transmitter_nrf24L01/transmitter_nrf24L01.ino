/*
 * NRF24L01       Arduino_ Uno    Arduino_Mega    Blue_Pill(stm32f01C)
 * __________________________________________________________________________
 * VCC        |       3.3v      |     3.3v      |     3.3v
 * GND        |       GND       |     GND       |      GND
 * CSN        |   Pin8  SPI/SS  | Pin10 SPI/SS  |     B12 NSS1 (PA4) 3.3v
 * CE         |   Pin7          | Pin9          |     PA8 digital (PB0) 3.3v
 * SCK        |   Pin13         | Pin52         |     B13 SCK1   (PA5) 3.3v
 * MOSI       |   Pin11         | Pin51         |     B15 MOSI1  (PA7) 3.3v
 * MISO       |   Pin12         | Pin50         |     B14 MISO1  (PA6) 3.3v
 * 
 * 
 */
#include <RF24.h>

RF24 radio(PA8, PB12); // CE, CSN    
const uint64_t address = 0xF0F0F0F0E1LL;
uint32_t timer;

void setup() {
Serial.begin(9600);
radio.begin();                  //Starting the Wireless communication
radio.openWritingPipe(address); //Setting the address where we will send the data
radio.setPALevel(RF24_PA_MIN);  //You can set it as minimum or maximum depending on the distance between the transmitter and receiver.
radio.setDataRate(RF24_2MBPS);
radio.stopListening();          //This sets the module as transmitter
}

void loop(){
  char text[10] = "";
  sprintf (text, "%d", random(20));

//  Serial.println(text);
  timer = micros();
  radio.write(&text, sizeof(text));                  //Sending the message to receiver
  Serial.println(micros() - timer);

}
