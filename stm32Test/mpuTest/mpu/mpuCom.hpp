/*
 * This code is designed for STM32f103c8t6 to communicate 
 * with JY901 which is an badass gyroscope combined with all kinds 
 * of sensors. This code can only be used in Arduino IDE. 
 *
 * During coding, the conundrums that I encountered are as follows.
 * It is impossible to use the original code provided by official 
 * manufaturer with STM32 in the Arduino IDE which, after all, is 
 * wrote for crappy Arduino microcontroller. So that I tried to
 * rewrite the provided code for STM32 in order to make it more 
 * compatible for Arduino IDE. This little chip has some weird IIC
 * communication features that I have to check for the datasheet
 * for details. And because of this reason, the wire library is 
 * not capable of handling the communication. I have to write the 
 * IIC communication method by hand...
 *
 * The last problem is a little bit tricky. The following delay function:
 *
 *  void Delay(u32 count) {
 *		while (count--);
 *  }
 *
 * which is low efficient but quite common in STM32 coding is not
 * working in the Arduino IDE, However, it works like a charm in 
 * STM32CubeIDE. Its likely that it has something to do with the
 * optimizer in the Arduino IDE.
 *
 */

#ifndef mpuCom_hpp
#define mpuCom_hpp
#include <stdint.h>
#define LED_ON          GPIOB->regs->ODR |= 1 << 12
#define LED_OFF         GPIOB->regs->ODR &= ~(1 << 12)

#define IIC_SCL_HIGH    GPIOB->regs->ODR |= 1 << 10  //SCL
#define IIC_SCL_LOW     GPIOB->regs->ODR &= ~(1 << 10)  //SCL

#define IIC_SDA_HIGH    GPIOB->regs->ODR |= 1 << 11 //SDA
#define IIC_SDA_LOW     GPIOB->regs->ODR &= ~(1 << 11) //SDA

#define SDA_IN()        {GPIOB->regs->CRH &= 0XFFFF0FFF;GPIOB->regs->CRH |= 0x00008000;}
#define SDA_OUT()       {GPIOB->regs->CRH &= 0XFFFF0FFF;GPIOB->regs->CRH |= 0x00003000;}

#define READ_SDA        GPIOB->regs->IDR & 0x800 //read SDA

void IIC_Start(void);
void IIC_Stop(void);
uint8_t IIC_Wait_Ack(void);
void IIC_Ack(void);
void IIC_NAck(void);
void IIC_Send_Byte(uint8_t txd);
uint8_t IIC_Read_Byte(unsigned char ack);
uint8_t IICreadBytes(uint8_t dev, uint8_t reg, uint8_t length, uint8_t *data);

#endif
