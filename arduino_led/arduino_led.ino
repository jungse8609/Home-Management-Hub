#include <SoftwareSerial.h>

SoftwareSerial BTSerial(10, 11); // RX, TX (블루투스 모듈 연결 핀)

// LED Pin
int RED = 9;
int GREEN = 8;
int BLUE = 7;
  
void setup() 
{
    Serial.begin(9600);     // PC 시리얼 모니터 통신
    BTSerial.begin(9600);   // HC-06 블루투스 모듈 통신 속도 설정

    // LED 핀 초기화
    pinMode(RED, OUTPUT); 
    pinMode(GREEN, OUTPUT);
    pinMode(BLUE, OUTPUT);
    
    analogWrite(RED, 0);
    analogWrite(GREEN, 0);
    analogWrite(BLUE, 0);

    Serial.println("LED Control Receiver Ready");
}
  
void loop() 
{
    // HC-06으로부터 데이터 수신
    if (BTSerial.available()) 
    {
        String command = BTSerial.readStringUntil('\n');
        command.trim();
        
        Serial.println("Received Command: " + command);

        // 명령에 따라 LED 제어
        if (command == "ON") 
        {
            // analogWrite(RED, 255);
            // analogWrite(GREEN, 255);
            // analogWrite(BLUE, 255);
            
            BTSerial.println("LED is ON");
            Serial.println("LED is ON");
        } 
        else if (command == "OFF") 
        {
            // analogWrite(RED, 0);
            // analogWrite(GREEN, 0);
            // analogWrite(BLUE, 0);
            
            BTSerial.println("LED is OFF");
            Serial.println("LED is OFF");
        } 
        else 
        {
            Serial.println("Invalid command received.");
        }
    }
}