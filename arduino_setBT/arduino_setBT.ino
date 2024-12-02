/*
신호를 받아 LED를 키고 끄는 제어를 담당
*/

#include <SoftwareSerial.h>

SoftwareSerial BTSerial(2,3); // 2(TX),3(RX)

// LED Pin
int RED = 11;
int GREEN = 10;
int BLUE = 9;
  
void setup()
{
  // Init HC-06
  Serial.begin(9600);
  BTSerial.begin(9600);

  // Init LED
  // pinMode(RED, OUTPUT); 
  // pinMode(GREEN, OUTPUT);
  // pinMode(BLUE, OUTPUT);
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
  
  // analogWrite(RED, 255); // RED ON
  // analogWrite(GREEN, 0);
  // analogWrite(BLUE, 0);
  // delay(1000);
   
  // analogWrite(RED, 0);
  // analogWrite(GREEN, 255); // GREEN ON
  // analogWrite(BLUE, 0);  
  // delay(1000);
 
  // analogWrite(RED, 0);
  // analogWrite(GREEN, 0);
  // analogWrite(BLUE, 255);    // BLUE ON
  // delay(1000);
}