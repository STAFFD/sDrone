void setup(){

  Serial.begin(115200);

  
  pinMode(PC13, OUTPUT);
//  digitalWrite(PC13, HIGH);
//  delay(100);
//  digitalWrite(PC13, LOW);
}

void loop(){
//  if(USART_RX_STA&0x8000) {
//    len=USART_RX_STA&0x3FFF;//得到此次接收到的数据长度 printf("\r\n 您发送的消息为:\r\n\r\n");
//    for(t=0;t<len;t++)
//    {
//    USART1->DR=USART_RX_BUF[t];
//    while((USART1->SR&0X40)==0);//等待发送结束 }
//    printf("\r\n\r\n");//插入换行
//    USART_RX_STA=0; }else
//    {
//    times++;
//    if(times%5000==0) {
//    printf("\r\n 战舰 STM32 开发板 串口实验\r\n");
//    printf("正点原子@ALIENTEK\r\n\r\n"); }
//    if(times%200==0)printf("请输入数据,以回车键结束\r\n"); if(times%30==0)LED0=!LED0;//闪烁 LED,提示系统正在运行. delay_ms(10);
//   }
}

void USART3_IRQHandler()
{
    //handle interrupt
}
