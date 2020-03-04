#ifndef SRDRONE_H_
#define SRDRONE_H_

#include <Wire.h>                          //Include the Wire.h library so we can communicate with the gyro.
#include <EEPROM.h>                        //Include the EEPROM.h library so we can store information onto the EEPROM

// output 7 for esc 1 (front-right - CCW) -> PORTD &= B01111111; 
// output 8 for esc 2 (rear-right - CW) -> PORTD &= B10111111; 
// output 9 for esc 3 (rear-left - CCW) -> PORTB &= B11111110; 
// output 10 for esc 4 (front-left - CW) -> PORTB &= B11111101; 
    
#define THROTTLE      3
#define YAW           0
#define PITCH         1
#define ROLL          2
#define TWIST         5
#define SWITCH        4
#define ON            true
#define OFF           false
#define ALL_UP        true
#define ALL_DOWN      false
#define BATTERY_PIN   A0

#define PULL_DOWN_ESC1  PORTD &= B01111111
#define PULL_DOWN_ESC2  PORTC &= B10111111 
#define PULL_DOWN_ESC3  PORTB &= B10111111
#define PULL_DOWN_ESC4  PORTB &= B11011111

//#define TWIST_ON
//#define SWITCH_ON
//#define DEBUG
//#define DEBUG_GYRO
//#define DEBUG_ANGLE
//#define DEBUG_RECEIVER
//#define DEBUG_BATTERY

#define PITCH_ACC_CORRECTION 3.915
#define ROLL_ACC_CORRECTION -0.894
#endif
