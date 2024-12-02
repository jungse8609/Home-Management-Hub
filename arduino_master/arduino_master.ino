#include <SoftwareSerial.h>

SoftwareSerial BTSerial(3, 4); // RX, TX

void setup() 
{
    Serial.begin(9600);     // PC 시리얼 모니터 통신
    BTSerial.begin(9600);   // HC-06 블루투스 모듈 통신 속도 설정
    
    Serial.println("Bluetooth LED Control");
    Serial.println("Type 'ON' or 'OFF' in Serial Monitor");
}

void loop() 
{
    // PC 시리얼 모니터로부터 입력 받기
    if (Serial.available()) 
    {
        String command = Serial.readStringUntil('\n');
        command.trim();

        // 유효한 명령인 경우
        if (command == "ON" || command == "OFF") 
        {
            Serial.println("Sending: " + command);
            BTSerial.println(command + "\n"); // 블루투스로 명령 전송 (개행 추가)
        } 
        else 
        {
            Serial.println("Invalid command. Use 'ON' or 'OFF'.");
        }
    }

    // 블루투스 수신 디버깅 (옵션)
    if (BTSerial.available()) 
    {
        String response = BTSerial.readStringUntil('\n');
        Serial.println("Received from Receiver: " + response);
    }
}