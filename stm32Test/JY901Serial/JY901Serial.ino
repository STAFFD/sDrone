#include <Wire.h>
#include <JY901.h>
/*
http://item.taobao.com/item.htm?id=43511899945
Test on mega2560.
JY901   mega2560
TX <---> 0(Rx)
*/

uint32_t timer;
bool flag = false;
void setup() 
{
  Serial2.begin(115200);
  Serial.begin(57600);  
  timer = micros();
}

void loop() 
{
  //print received data. Data was received in Serial2Event;
//  Serial2.print("Time:20");Serial2.print(JY901.stcTime.ucYear);Serial2.print("-");Serial2.print(JY901.stcTime.ucMonth);Serial2.print("-");Serial2.print(JY901.stcTime.ucDay);
//  Serial2.print(" ");Serial2.print(JY901.stcTime.ucHour);Serial2.print(":");Serial2.print(JY901.stcTime.ucMinute);Serial2.print(":");Serial2.println((float)JY901.stcTime.ucSecond+(float)JY901.stcTime.usMiliSecond/1000);
//               
//  Serial2.print("Acc:");Serial2.print((float)JY901.stcAcc.a[0]/32768*16);Serial2.print(" ");Serial2.print((float)JY901.stcAcc.a[1]/32768*16);Serial2.print(" ");Serial2.println((float)JY901.stcAcc.a[2]/32768*16);
  
//  Serial2.print("Gyro:");Serial2.print((float)JY901.stcGyro.w[0]/32768*2000);Serial2.print(" ");Serial2.print((float)JY901.stcGyro.w[1]/32768*2000);Serial2.print(" ");Serial2.println((float)JY901.stcGyro.w[2]/32768*2000);
  
  Serial.print("Angle:");Serial.print((float)JY901.stcAngle.Angle[0]/32768*180);Serial.print(" ");Serial.print((float)JY901.stcAngle.Angle[1]/32768*180);Serial.print(" ");Serial.println((float)JY901.stcAngle.Angle[2]/32768*180);
  
//  Serial2.print("Mag:");Serial2.print(JY901.stcMag.h[0]);Serial2.print(" ");Serial2.print(JY901.stcMag.h[1]);Serial2.print(" ");Serial2.println(JY901.stcMag.h[2]);
//  
//  Serial2.print("Pressure:");Serial2.print(JY901.stcPress.lPressure);Serial2.print(" ");Serial2.println((float)JY901.stcPress.lAltitude/100);
//  
//  Serial2.print("DStatus:");Serial2.print(JY901.stcDStatus.sDStatus[0]);Serial2.print(" ");Serial2.print(JY901.stcDStatus.sDStatus[1]);Serial2.print(" ");Serial2.print(JY901.stcDStatus.sDStatus[2]);Serial2.print(" ");Serial2.println(JY901.stcDStatus.sDStatus[3]);
//  
//  Serial2.print("Longitude:");Serial2.print(JY901.stcLonLat.lLon/10000000);Serial2.print("Deg");Serial2.print((double)(JY901.stcLonLat.lLon % 10000000)/1e5);Serial2.print("m Lattitude:");
//  Serial2.print(JY901.stcLonLat.lLat/10000000);Serial2.print("Deg");Serial2.print((double)(JY901.stcLonLat.lLat % 10000000)/1e5);Serial2.println("m");
//  
//  Serial2.print("GPSHeight:");Serial2.print((float)JY901.stcGPSV.sGPSHeight/10);Serial2.print("m GPSYaw:");Serial2.print((float)JY901.stcGPSV.sGPSYaw/10);Serial2.print("Deg GPSV:");Serial2.print((float)JY901.stcGPSV.lGPSVelocity/1000);Serial2.println("km/h");
//  
//  Serial2.print("SN:");Serial2.print(JY901.stcSN.sSVNum);Serial2.print(" PDOP:");Serial2.print((float)JY901.stcSN.sPDOP/100);Serial2.print(" HDOP:");Serial2.print((float)JY901.stcSN.sHDOP/100);Serial2.print(" VDOP:");Serial2.println((float)JY901.stcSN.sVDOP/100);
  
  Serial.println("");
//  delay(4);

  while (Serial2.available()) {
//    if (!flag){
//     Serial2.println(micros() - timer);
//     timer = micros();
//     flag = true;
//    }
    JY901.CopeSerialData(Serial2.read()); //Call JY901 data cope function
  }
  flag = false;

}
