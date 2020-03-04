uint32_t timer;

void setup(){
  Serial.begin(57600);
  Serial2.begin(57600);
}

void loop(){
  Serial.print("sending..");
  timer = micros();
  Serial2.println("weougbovwurbovwrbvworbw oe bwoe ubfwoeu bfwqoeb o bqo");
  Serial.println(micros() - timer);
  delay(50);
}
