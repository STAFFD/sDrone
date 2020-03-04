#include <JY901.h>

#define THROTTLE      2
#define YAW           3
#define PITCH         1
#define ROLL          0

#define OFF LOW
#define ON HIGH

int receiver_input[4];
bool last_active_channel[4];
volatile int  receive_channel_previous[4];
unsigned long current_time;
unsigned long loopTime;
int start;
void hints_init(){
  DDRB |= B00010000;              //Configure digital poort 13 as output.

}

void LED(bool on){
  if(on) PORTB |= B00010000;  
  else PORTB &= B11101111;  
}

void LED_toggle(){
  if(PINB & B00010000 ) LED(OFF);
  else LED(ON);
}

void setup(){
  Serial.begin(9600);
  PCICR |= (1 << PCIE0);                                                    //Set PCIE0 to enable PCMSK0 scan.
  PCMSK0 |= (1 << PCINT0);                                                  //Set PCINT0 (digital input 8) to trigger an interrupt on state change.
  PCMSK0 |= (1 << PCINT1);                                                  //Set PCINT1 (digital input 9)to trigger an interrupt on state change.
  PCMSK0 |= (1 << PCINT2);                                                  //Set PCINT2 (digital input 10)to trigger an interrupt on state change.
  PCMSK0 |= (1 << PCINT3);  
  hints_init();
  receiver_check();  
  JY901.StartIIC();
}

void receiver_check(){
  //Wait until the receiver is active and the throtle is set to the lower position.
  while(receiver_input[THROTTLE] < 990 || receiver_input[THROTTLE] > 1020 || receiver_input[YAW] < 1400){
    start ++;                                                               //While waiting increment start whith every loop.
    //Wait until the receiver is active and the throtle is set to the lower position.
//    pull_esc(ALL_UP);
//    delayMicroseconds(1000);                                                //Wait 1000us.
//    pull_esc(ALL_DOWN);
    if(start == 125){                                                       //Every 125 loops (500ms).
      LED_toggle();                                   //Change the led status.
      start = 0;                                                            //Start again at 0.
    }
  }
  start = 0;                                                                //Set start back to 0.  
}

void loop(){
//    Serial.print("THROTTLE: ");
//    Serial.print(receiver_input[THROTTLE]);
//    Serial.print(" YAW: ");
//    Serial.print(receiver_input[YAW]);
//    Serial.print(" PITCH: ");
//    Serial.print(receiver_input[PITCH]);
//    Serial.print(" ROLL: ");
//    Serial.println(receiver_input[ROLL]);  

  
  loopTime = micros();
  while (Serial.available()) 
  {
    JY901.CopeSerialData(Serial.read()); //Call JY901 data cope function
  }
  loopTime = micros() - loopTime;
// Serial.print("Gyro:");Serial.print((float)JY901.stcGyro.w[0]/32768*2000);Serial.print(" ");Serial.print((float)JY901.stcGyro.w[1]/32768*2000);Serial.print(" ");Serial.println((float)JY901.stcGyro.w[2]/32768*2000);
//  Serial.print("loopTime: ");Serial.println(loopTime);
//  Serial.print("ADDR: ");Serial.println(JY901.ReadWord(0x1a));
  Serial.print("Angle:");Serial.print((float)JY901.stcAngle.Angle[0]/32768*180);Serial.print(" ");Serial.print((float)JY901.stcAngle.Angle[1]/32768*180);Serial.print(" ");Serial.println((float)JY901.stcAngle.Angle[2]/32768*180);
//  
//  Serial.print("Mag:");Serial.print(JY901.stcMag.h[0]);Serial.print(" ");Serial.print(JY901.stcMag.h[1]);Serial.print(" ");Serial.println(JY901.stcMag.h[2]);
    
    delay(100);
}

ISR(PCINT0_vect){
  current_time = micros();
  //Channel 1=========================================
  if(PINB & 1 << 0){                                                     //Is input 8 high?
    if(!last_active_channel[0]){                                                //Input 8 changed from 0 to 1.
      last_active_channel[0] = true;                                                   //Remember current input state.
      receive_channel_previous[0] = current_time;                                               //Set timer_1 to current_time.
    }
  }
  else if(last_active_channel[0]){                                             //Input 8 is not high and changed from 1 to 0.
    last_active_channel[0] = false;                                                     //Remember current input state.
    receiver_input[0] = current_time - receive_channel_previous[0];                             //Channel 1 is current_time - timer_1.
  }

  //Channel 2=========================================
  if(PINB & 1 << 1 ){                                                    //Is input 9 high?
      if(!last_active_channel[1]){                                                //Input 9 changed from 0 to 1.
        last_active_channel[1] = true;                                                   //Remember current input state.
        receive_channel_previous[1] = current_time;                                               //Set timer_2 to current_time.
      }
  }
    else if(last_active_channel[1]){                                             //Input 9 is not high and changed from 1 to 0.
      last_active_channel[1] = false;                                                     //Remember current input state.
      receiver_input[1] = current_time - receive_channel_previous[1];                             //Channel 2 is current_time - timer_2.
    }
  
    //Channel 3=========================================
    if(PINB & 1 << 2 ){                                                    //Is input 10 high?
      if(!last_active_channel[2]){                                                //Input 10 changed from 0 to 1.
        last_active_channel[2] = true;                                                   //Remember current input state.
        receive_channel_previous[2] = current_time;                                               //Set timer_3 to current_time.
      }
    }
    else if(last_active_channel[2]){                                             //Input 10 is not high and changed from 1 to 0.
      last_active_channel[2] = false;                                                     //Remember current input state.
      receiver_input[2] = current_time - receive_channel_previous[2];                             //Channel 3 is current_time - timer_3.
  
    }

      //Channel 3=========================================
    if(PINB & 1 << 3 ){                                                    //Is input 10 high?
      if(!last_active_channel[3]){                                                //Input 10 changed from 0 to 1.
        last_active_channel[3] = true;                                                   //Remember current input state.
        receive_channel_previous[3] = current_time;                                               //Set timer_3 to current_time.
      }
    }
    else if(last_active_channel[3]){                                             //Input 10 is not high and changed from 1 to 0.
      last_active_channel[3] = false;                                                     //Remember current input state.
      receiver_input[3] = current_time - receive_channel_previous[3];                             //Channel 3 is current_time - timer_3.
  
    }

}
