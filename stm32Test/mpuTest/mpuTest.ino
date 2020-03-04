#include "REG.h"
#define gyro_address 0x50

#define BEEP_ON             GPIOB->regs->ODR |= 1 << 3
#define BEEP_OFF            GPIOB->regs->ODR &= ~(1 << 3)
#define BEEP_CHANGE_STATE() (GPIOB->regs->ODR & 1 << 3) ? BEEP_OFF : BEEP_ON

#define IIC_SCL_HIGH      GPIOB->regs->ODR |= 1 << 10  //SCL
#define IIC_SCL_LOW       GPIOB->regs->ODR &= ~(1 << 10)  //SCL

#define IIC_SDA_HIGH      GPIOB->regs->ODR |= 1 << 11 //SDA
#define IIC_SDA_LOW       GPIOB->regs->ODR &= ~(1 << 11) //SDA

#define SDA_IN()          {GPIOB->regs->CRH &= 0XFFFF0FFF;GPIOB->regs->CRH |= 0x00008000;}
#define SDA_OUT()         {GPIOB->regs->CRH &= 0XFFFF0FFF;GPIOB->regs->CRH |= 0x00003000;}

#define READ_SDA          GPIOB->regs->IDR & 0x800 //read SDA

unsigned char chrTemp[30];

float a[3],w[3],h[3],Angle[3];

short CharToShort(unsigned char cData[]) {
  return ((short)cData[1]<<8)|cData[0];
}

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
    (txd&0x80)>>7 ? IIC_SDA_HIGH : IIC_SDA_LOW;
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

void setup() {

  Serial.begin(115200);
  delay(250);
  afio_cfg_debug_ports(AFIO_DEBUG_SW_ONLY);  
  
  GPIOB->regs->CRL = 0x3000;
  GPIOB->regs->CRH = 0x33300; // set pin B12 , B11, B10 to ouput mode, push-pull mode, ouput speed 50MHZ
  

  BEEP_ON;  // pull pin B12 to high
  delay(100);
  BEEP_OFF; // pull pin B3 to low
}

void loop() {
    IICreadBytes(gyro_address, AX, 24,&chrTemp[0]);
    a[0] = (float)CharToShort(&chrTemp[0])/32768*16;
    a[1] = (float)CharToShort(&chrTemp[2])/32768*16;
    a[2] = (float)CharToShort(&chrTemp[4])/32768*16;
    w[0] = (float)CharToShort(&chrTemp[6])/32768*2000;
    w[1] = (float)CharToShort(&chrTemp[8])/32768*2000;
    w[2] = (float)CharToShort(&chrTemp[10])/32768*2000;
    h[0] = CharToShort(&chrTemp[12]);
    h[1] = CharToShort(&chrTemp[14]);
    h[2] = CharToShort(&chrTemp[16]);
    Angle[0] = (float)CharToShort(&chrTemp[18])/32768*180;
    Angle[1] = (float)CharToShort(&chrTemp[20])/32768*180;
    Angle[2] = (float)CharToShort(&chrTemp[22])/32768*180;

//    Serial.print("ax: ");
//    Serial.print(a[0]);
//    Serial.print(" ay: ");
//    Serial.print(a[1]);
//    Serial.print(" az: ");
//    Serial.println(a[2]);

//    Serial.print("h: ");
//    Serial.print(w[0]);
//    Serial.print("\r");
//    Serial.print(w[1]);
//    Serial.print("\r");
//    Serial.print(w[0]);
//
//    Serial.print("h: ");
//    Serial.print(h[0]);
//    Serial.print("\r");
//    Serial.print(h[1]);
//    Serial.print("\r");
//    Serial.print(h[0]);
//    
    Serial.print("Angelx: ");
    Serial.print(Angle[0]);
    Serial.print(" Angely: ");
    Serial.print(Angle[1]);
    Serial.print(" Angelz: ");
    Serial.println(Angle[2]);
    delay(10);
    
}
