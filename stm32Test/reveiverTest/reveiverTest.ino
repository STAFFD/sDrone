#include <libmaple/scb.h>
#include <JY901.h>
#define THROTTLE_INPUT  2
#define YAW_INPUT       3
#define PITCH_INPUT     1
#define ROLL_INPUT      0
#define TWIST_INPUT     5
#define SWITCH_INPUT    4

#define IMU_SCALE                       32768
#define GYRO_SCALE                      2000
#define ANGLE_SCALE                     180


#define LED_RED_ON              GPIOB->regs->ODR |= 1 << 4
#define LED_RED_OFF             GPIOB->regs->ODR &= ~(1 << 4)
#define LED_RED_CHANGE_STATE()  (GPIOB->regs->ODR & 1 << 4) ? LED_OFF : LED_ON

#define LED_GREEN_ON             GPIOB->regs->ODR |= 1 << 3
#define LED_GREEN_OFF            GPIOB->regs->ODR &= ~(1 << 3)
#define LED_GREEN_CHANGE_STATE() (GPIOB->regs->ODR & 1 << 3) ? BEEP_OFF : BEEP_ON

#define BEEP_ON             GPIOC->regs->ODR |= 1 << 13
#define BEEP_OFF            GPIOC->regs->ODR &= ~(1 << 13)
#define BEEP_CHANGE_STATE() (GPIOC->regs->ODR & 1 << 13) ? BEEP_OFF : BEEP_ON

int32_t reveiver_input_previous[6], receiver_input[6];
float gyro_roll_input, gyro_pitch_input, gyro_yaw_input, angle_roll, angle_pitch;
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//In this part the various registers of the MPU-6050 are set.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void gyro_setup(void){
  Serial2.begin(115200);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//This part reads the raw gyro and accelerometer data from the MPU-6050
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void gyro_signalen(void) {
  while (Serial2.available()) JY901.CopeSerialData(Serial2.read()); //Call JY901 data cope function
  gyro_roll_input  = -(float)JY901.stcGyro.w[1] / IMU_SCALE * GYRO_SCALE;
  gyro_pitch_input = -(float)JY901.stcGyro.w[0] / IMU_SCALE * GYRO_SCALE;
  gyro_yaw_input   = -(float)JY901.stcGyro.w[2] / IMU_SCALE * GYRO_SCALE;

  angle_roll  = -(float)JY901.stcAngle.Angle[1] / IMU_SCALE * ANGLE_SCALE;
  angle_pitch = -(float)JY901.stcAngle.Angle[0] / IMU_SCALE * ANGLE_SCALE;
}

void setup() {
  afio_cfg_debug_ports(AFIO_DEBUG_SW_ONLY);  
  GPIOB->regs->CRL = 0x11000;
  GPIOC->regs->CRH |= 0x100000;
  Serial.begin(57600);
  Timer2.attachCompare1Interrupt(handler_channel_1);
  Timer2.attachCompare2Interrupt(handler_channel_2);
  Timer2.attachCompare3Interrupt(handler_channel_3);
  Timer2.attachCompare4Interrupt(handler_channel_4);
  TIMER2_BASE->CR1 = TIMER_CR1_CEN;
  TIMER2_BASE->CR2 = 0;
  TIMER2_BASE->SMCR = 0;
  TIMER2_BASE->DIER = TIMER_DIER_CC1IE | TIMER_DIER_CC2IE | TIMER_DIER_CC3IE | TIMER_DIER_CC4IE;
  TIMER2_BASE->EGR = 0;
  TIMER2_BASE->CCMR1 = 0b100000001; //Register is set like this due to a bug in the define table (30-09-2017)
  TIMER2_BASE->CCMR2 = 0b100000001; //Register is set like this due to a bug in the define table (30-09-2017)
  TIMER2_BASE->CCER = TIMER_CCER_CC1E | TIMER_CCER_CC2E | TIMER_CCER_CC3E | TIMER_CCER_CC4E;
  TIMER2_BASE->PSC = 71;
  TIMER2_BASE->ARR = 0xFFFF;
  TIMER2_BASE->DCR = 0;

  Timer3.attachCompare1Interrupt(handler_channel_5);
  Timer3.attachCompare2Interrupt(handler_channel_6);
  TIMER3_BASE->CR1 = TIMER_CR1_CEN;
  TIMER3_BASE->CR2 = 0;
  TIMER3_BASE->SMCR = 0;
  TIMER3_BASE->DIER = TIMER_DIER_CC1IE | TIMER_DIER_CC2IE;
  TIMER3_BASE->EGR = 0;
  TIMER3_BASE->CCMR1 = 0b100000001; //Register is set like this due to a bug in the define table (30-09-2017)
  TIMER3_BASE->CCMR2 = 0;
  TIMER3_BASE->CCER = TIMER_CCER_CC1E | TIMER_CCER_CC2E;
  TIMER3_BASE->PSC = 71;
  TIMER3_BASE->ARR = 0xFFFF;
  TIMER3_BASE->DCR = 0;

  TIMER4_BASE->CR1 = TIMER_CR1_CEN | TIMER_CR1_ARPE;
  TIMER4_BASE->CR2 = 0;
  TIMER4_BASE->SMCR = 0;
  TIMER4_BASE->DIER = 0;
  TIMER4_BASE->EGR = 0;
  TIMER4_BASE->CCMR1 = (0b110 << 4) | TIMER_CCMR1_OC1PE |(0b110 << 12) | TIMER_CCMR1_OC2PE;
  TIMER4_BASE->CCMR2 = (0b110 << 4) | TIMER_CCMR2_OC3PE |(0b110 << 12) | TIMER_CCMR2_OC4PE;
  TIMER4_BASE->CCER = TIMER_CCER_CC1E | TIMER_CCER_CC2E | TIMER_CCER_CC3E | TIMER_CCER_CC4E;
  TIMER4_BASE->PSC = 71;
  TIMER4_BASE->ARR = 5000;
  TIMER4_BASE->DCR = 0;
  TIMER4_BASE->CCR1 = 1000;

  TIMER4_BASE->CCR1 = 1000;
  TIMER4_BASE->CCR2 = 1000;
  TIMER4_BASE->CCR3 = 1000;
  TIMER4_BASE->CCR4 = 1000;
  pinMode(PB6, PWM);
  pinMode(PB7, PWM);
  pinMode(PB8, PWM);
  pinMode(PB9, PWM);

  gyro_setup();
}

// the loop function runs over and over again forever
void loop() {
  delayMicroseconds(3500);
  Serial.print("THROTTLE_INPUT:");
  Serial.print(receiver_input[THROTTLE_INPUT]);
  Serial.print(" YAW_INPUT:");
  Serial.print(receiver_input[YAW_INPUT]);
  Serial.print(" PITCH_INPUT:");
  Serial.print(receiver_input[PITCH_INPUT]);
  Serial.print(" ROLL_INPUT:");
  Serial.print(receiver_input[ROLL_INPUT]);
  Serial.print(" TWIST_INPUT:");
  Serial.print(receiver_input[TWIST_INPUT]);
  Serial.print(" SWITCH_INPUT:");
  Serial.println(receiver_input[SWITCH_INPUT]);

  gyro_signalen();
  BEEP_CHANGE_STATE();
  delay(500);
}

void handler_channel_1(void) {                           //This function is called when channel 1 is captured.
  if (0b1 & GPIOA_BASE->IDR  >> 0) {                     //If the receiver channel 1 input pulse on A0 is high.
    reveiver_input_previous[0] = TIMER2_BASE->CCR1;                 //Record the start time of the pulse.
    TIMER2_BASE->CCER |= TIMER_CCER_CC1P;                //Change the input capture mode to the falling edge of the pulse.
  }
  else {                                                 //If the receiver channel 1 input pulse on A0 is low.
    receiver_input[0] = TIMER2_BASE->CCR1 - reveiver_input_previous[0];     //Calculate the total pulse time.
    if (receiver_input[0] < 0)receiver_input[0] += 0xFFFF;               //If the timer has rolled over a correction is needed.
    TIMER2_BASE->CCER &= ~TIMER_CCER_CC1P;               //Change the input capture mode to the rising edge of the pulse.
  }
}

void handler_channel_2(void) {                           //This function is called when channel 2 is captured.
  if (0b1 & GPIOA_BASE->IDR >> 1) {                      //If the receiver channel 2 input pulse on A1 is high.
    reveiver_input_previous[1] = TIMER2_BASE->CCR2;                 //Record the start time of the pulse.
    TIMER2_BASE->CCER |= TIMER_CCER_CC2P;                //Change the input capture mode to the falling edge of the pulse.
  }
  else {                                                 //If the receiver channel 2 input pulse on A1 is low.
    receiver_input[1] = TIMER2_BASE->CCR2 - reveiver_input_previous[1];     //Calculate the total pulse time.
    if (receiver_input[1] < 0)receiver_input[1] += 0xFFFF;               //If the timer has rolled over a correction is needed.
    TIMER2_BASE->CCER &= ~TIMER_CCER_CC2P;               //Change the input capture mode to the rising edge of the pulse.
  }
}

void handler_channel_3(void) {                           //This function is called when channel 3 is captured.
  if (0b1 & GPIOA_BASE->IDR >> 2) {                      //If the receiver channel 3 input pulse on A2 is high.
    reveiver_input_previous[2] = TIMER2_BASE->CCR3;                 //Record the start time of the pulse.
    TIMER2_BASE->CCER |= TIMER_CCER_CC3P;                //Change the input capture mode to the falling edge of the pulse.
  }
  else {                                                 //If the receiver channel 3 input pulse on A2 is low.
    receiver_input[2] = TIMER2_BASE->CCR3 - reveiver_input_previous[2];     //Calculate the total pulse time.
    if (receiver_input[2] < 0)receiver_input[2] += 0xFFFF;               //If the timer has rolled over a correction is needed.
    TIMER2_BASE->CCER &= ~TIMER_CCER_CC3P;               //Change the input capture mode to the rising edge of the pulse.
  }
}

void handler_channel_4(void) {                           //This function is called when channel 4 is captured.
  if (0b1 & GPIOA_BASE->IDR >> 3) {                      //If the receiver channel 4 input pulse on A3 is high.
    reveiver_input_previous[3] = TIMER2_BASE->CCR4;                 //Record the start time of the pulse.
    TIMER2_BASE->CCER |= TIMER_CCER_CC4P;                //Change the input capture mode to the falling edge of the pulse.
  }
  else {                                                 //If the receiver channel 4 input pulse on A3 is low.
    receiver_input[3] = TIMER2_BASE->CCR4 - reveiver_input_previous[3];     //Calculate the total pulse time.
    if (receiver_input[3] < 0)receiver_input[3] += 0xFFFF;               //If the timer has rolled over a correction is needed.
    TIMER2_BASE->CCER &= ~TIMER_CCER_CC4P;               //Change the input capture mode to the rising edge of the pulse.
  }
}

void handler_channel_5(void) {                           //This function is called when channel 5 is captured.
  if (0b1 & GPIOA_BASE->IDR >> 6) {                      //If the receiver channel 5 input pulse on A6 is high.
    reveiver_input_previous[4] = TIMER3_BASE->CCR1;                 //Record the start time of the pulse.
    TIMER3_BASE->CCER |= TIMER_CCER_CC1P;                //Change the input capture mode to the falling edge of the pulse.
  }
  else {                                                 //If the receiver channel 5 input pulse on A6 is low.
    receiver_input[4] = TIMER3_BASE->CCR1 - reveiver_input_previous[4];     //Calculate the total pulse time.
    if (receiver_input[4] < 0)receiver_input[4] += 0xFFFF;               //If the timer has rolled over a correction is needed.
    TIMER3_BASE->CCER &= ~TIMER_CCER_CC1P;               //Change the input capture mode to the rising edge of the pulse.
  }
}

void handler_channel_6(void) {                           //This function is called when channel 6 is captured.
  if (0b1 & GPIOA_BASE->IDR >> 7) {                      //If the receiver channel 6 input pulse on A7 is high.
    reveiver_input_previous[5] = TIMER3_BASE->CCR2;                 //Record the start time of the pulse.
    TIMER3_BASE->CCER |= TIMER_CCER_CC2P;                //Change the input capture mode to the falling edge of the pulse.
  }
  else {                                                 //If the receiver channel 6 input pulse on A7 is low.
    receiver_input[5] = TIMER3_BASE->CCR2 - reveiver_input_previous[5];     //Calculate the total pulse time.
    if (receiver_input[5] < 0)receiver_input[5] += 0xFFFF;               //If the timer has rolled over a correction is needed.
    TIMER3_BASE->CCER &= ~TIMER_CCER_CC2P;               //Change the input capture mode to the rising edge of the pulse.
  }
}
