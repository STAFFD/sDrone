#ifndef TELEGRAM_H_    
#define TELEGRAM_H_


#define CHECK_BIT_ONE 1
#define CHECK_BIT_TWO 2
#define LATTITUDE     3
#define LONGITUDE     4
#define BATTERY       5

//#define CHECK_BIT_TWO 12
//#define CHECK_BIT_TWO 13
//#define CHECK_BIT_TWO 14
//#define CHECK_BIT_TWO 15
//#define CHECK_BIT_TWO 16

#define TELEGRAM_TX_HIGH  GPIOA_BASE->BSRR = 0b1 << 9
#define TELEGRAM_TX_LOW   GPIOA_BASE->BSRR = 0b1 << (16 + 9)
#define TELEGRAM_TX_START TELEGRAM_TX_LOW
#define TELEGRAM_TX_STOP  TELEGRAM_TX_HIGH

#endif
