#include <libmaple/scb.h>

uint32_t present[4], systick_counter[4], reveiver_input_previous[4], receiver_input[4];

bool flags[4];

void setup() {
  Serial.begin(57600);
  attachInterrupt(PB10, receiver_channel_1, CHANGE);
}

// the loop function runs over and over again forever
void loop() {
  delayMicroseconds(3500);
  Serial.print("receiver_channel_1:");
  Serial.print(receiver_input[0]);
  Serial.print(" receiver_channel_2:");
  Serial.print(receiver_input[1]);
  Serial.print(" receiver_channel_3:");
  Serial.print(receiver_input[2]);
  Serial.print(" receiver_channel_4:");
  Serial.println(receiver_input[3]);
}

void receiver_channel_1(){
  systick_counter[0] = SYSTICK_BASE -> CNT;

    if(0b1 & SCB_BASE->ICSR >> 26){
      flags[0] = true;
      systick_counter[0] = SYSTICK_BASE -> CNT;
    } else flags[0] = false;

    [0] = (systick_uptime_millis * 1000) + (SYSTICK_RELOAD_VAL + 1 - systick_counter[0]) / CYCLES_PER_MICROSECOND;

    if(flags[0]) present[0] += 1000;

    if(0B1 & GPIOB_BASE -> IDR >> 10) reveiver_input_previous[0] = present[0];
    else receiver_input[0] = present[0] - reveiver_input_previous[0];
}
