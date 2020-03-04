#include <RF24.h>

RF24 radio(7,8);

const uint64_t address = 0xF0F0F0F0E1LL;

void setup() {
  // put your setup code here, to run once:
  radio.begin();
  radio.openReadingPipe(0,address);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();

  Serial.begin(9600);
}

void loop() {
  char text[50] = "";
  
  if (radio.available()) {
    radio.read(&text, sizeof(text));
    Serial.println(text);
  }
  delay(10);
}
