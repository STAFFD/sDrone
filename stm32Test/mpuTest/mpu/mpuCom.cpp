#include "mpuCom.hpp"

void IIC_Start(void) {
  SDA_OUT();     
  IIC_SDA_HIGH;
  IIC_SCL_HIGH;
  delayMicroseconds(5);
  IIC_SDA_LOW;//START:when CLK is high,DATA change form high to low
  delayMicroseconds(5);
  IIC_SCL_LOW;
}

void IIC_Stop(void) {
  SDA_OUT();
  IIC_SCL_LOW;
  IIC_SDA_LOW;//STOP:when CLK is high DATA change form low to high
  delayMicroseconds(5);
  IIC_SCL_HIGH;
  IIC_SDA_HIGH;
  delayMicroseconds(5);
}

uint8_t IIC_Wait_Ack(void){
  uint8_t ucErrTime=0;
  SDA_IN(); 
  IIC_SDA_HIGH;
  delayMicroseconds(5);
  while(READ_SDA) {
    ucErrTime++;
    if(ucErrTime>50){
      IIC_Stop();
      return 1;
    }
    delayMicroseconds(5);
  }
  IIC_SCL_HIGH;
  delayMicroseconds(5);
  IIC_SCL_LOW;
  return 0;
}

void IIC_Ack(void) {
  IIC_SCL_LOW;
  SDA_OUT();
  IIC_SDA_LOW;
  delayMicroseconds(5);
  IIC_SCL_HIGH;
  delayMicroseconds(5);
  IIC_SCL_LOW;
}

void IIC_NAck(void) {
  IIC_SCL_LOW;
  SDA_OUT();
  IIC_SDA_HIGH;
  delayMicroseconds(5);
  IIC_SCL_HIGH;
  delayMicroseconds(5);
  IIC_SCL_LOW;
}

void IIC_Send_Byte(uint8_t txd) {
  SDA_OUT();
  IIC_SCL_LOW;
  for(uint8_t t=0; t<8; t++){
    (txd&0x80)>>7 : IIC_SDA_HIGH ? IIC_SDA_LOW;
    txd<<=1;
    delayMicroseconds(2);
    IIC_SCL_HIGH;
    delayMicroseconds(5);
    IIC_SCL_LOW;
    delayMicroseconds(3);
  }
}

uint8_t IIC_Read_Byte(unsigned char ack) {
  unsigned char count, receive = 0;
  SDA_IN();
  for(uint8_t count=0;count<8;count++ ){
    IIC_SCL_LOW;
    delayMicroseconds(5);
    IIC_SCL_HIGH;
    receive<<=1;
    if(READ_SDA)receive++;
    delayMicroseconds(5);
  }
  if (ack) IIC_Ack();
  else IIC_NAck();
  return receive;
}

uint8_t IICreadBytes(uint8_t dev, uint8_t reg, uint8_t length, uint8_t *data){
  uint8_t count = 0;
  IIC_Start();
  IIC_Send_Byte(dev<<1); 
  IIC_Wait_Ack();
  IIC_Send_Byte(reg);
  IIC_Wait_Ack();
  IIC_Start();
  IIC_Send_Byte((dev<<1)+1);
  IIC_Wait_Ack();
  for(count=0;count<length;count++){
    if(count!=length-1) data[count]=IIC_Read_Byte(1);
    else  data[count]=IIC_Read_Byte(0);
  }
  IIC_Stop();
  return count;
}
