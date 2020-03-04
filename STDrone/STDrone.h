#ifndef STDRONE_H_    
#define STDRONE_H_
#include <JY901.h>
#include <libmaple/scb.h>
#include "telegram.h"

#define LOOP_TIME_MIN 4000
#define LOOP_TIME_MAX 4050

#define MOTOR_STOP 1000
#define MOTOR_MIN  1100
#define MOTOR_MAX  2000

//#define DEBUG
//#define DEBUG_HARDWARE
//#define DEBUG_BLUETOOTH
//#define DEBUG_GPS
//#define DEBUG_CAMPASS
//#define DEBUG_CAMPASS_RAW
//#define DEBUG_GYRO
//#define DEBUG_LOOP_TIME
//#define DEBUG_ANGLE
//#define DEBUG_RECEIVER
//#define DEBUG_BATTERY

#define PPM_SIGNAL
#define THROTTLE_INPUT  2
#define YAW_INPUT       3
#define PITCH_INPUT     1
#define ROLL_INPUT      0
#define TWIST_INPUT     5
#define SWITCH_INPUT    4

#define IMU_SCALE                       32768
#define GYRO_SCALE                      2000
#define ANGLE_SCALE                     180
#define ANGLE_ROLL_CALIBRATE_VALUE      0.89
#define ANGLE_PITCH_CALIBRATE_VALUE     -0.3
#define ANGLE_YAW_CALIBRATE_VALUE       180//141.1

#define LED_RED_ON              GPIOC->regs->ODR |= 1 << 14
#define LED_RED_OFF             GPIOC->regs->ODR &= ~(1 << 14)
#define LED_RED_CHANGE_STATE()  (GPIOC->regs->ODR & 1 << 14) ? LED_OFF : LED_ON

#define LED_GREEN_ON             GPIOC->regs->ODR |= 1 << 15
#define LED_GREEN_OFF            GPIOC->regs->ODR &= ~(1 << 15)
#define LED_GREEN_CHANGE_STATE() (GPIOC->regs->ODR & 1 << 15) ? BEEP_OFF : BEEP_ON

#define BEEP_ON             GPIOC->regs->ODR |= 1 << 13
#define BEEP_OFF            GPIOC->regs->ODR &= ~(1 << 13)
#define BEEP_CHANGE_STATE() (GPIOC->regs->ODR & 1 << 13) ? BEEP_OFF : BEEP_ON

#define ON  HIGH
#define OFF LOW

#define BATTERY_PIN   PA7
#endif
