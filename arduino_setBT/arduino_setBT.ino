/*
블루투스 모듈 세팅
*/

#include <SoftwareSerial.h>

SoftwareSerial BTSerial(10,11); // 2(TX),3(RX)
  
void setup()
{
  // Init HC-06
  Serial.begin(9600);
  BTSerial.begin(9600);
}
  
void loop()
{
  if(Serial.available()) // 시리얼 통신으로 문자가 들어오면
  {
    BTSerial.write(Serial.read()); // 블루투스시리얼 통신으로 발송
  }
  if(BTSerial.available()) // 블루투스 시리얼 통신으로 문자가 들어오면
  {
    Serial.write(BTSerial.read()); // 시리얼 창에 표시(시리얼 통신으로 출력)
  }
}