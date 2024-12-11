#include <SoftwareSerial.h> // 블루투스 모듈 라이브러리

// BLUE_PINtooth
SoftwareSerial BTSerial(10,11); // 2(TX),3(RX)

// LED Pin
int RED_PIN = 6;
int GREEN_PIN = 7;
int BLUE_PIN = 8;

void setup() {
  Serial.begin(9600);     // PC 시리얼 모니터 통신
  BTSerial.begin(9600);   // HC-06 블루투스 모듈 통신 속도 설정
  
  // LED 핀 초기화
  pinMode(RED_PIN, OUTPUT); 
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);  
  analogWrite(RED_PIN, 255);
  analogWrite(GREEN_PIN, 255);
  analogWrite(BLUE_PIN, 255);
}

void loop() 
{
  if(Serial.available()) // 시리얼 통신으로 문자가 들어오면
  {
    BTSerial.write(Serial.read()); // 블루투스시리얼 통신으로 발송
  }
  if(BTSerial.available()) // 블루투스 시리얼 통신으로 문자가 들어오면
  {
    String command = "";
    while (BTSerial.available()) 
    {
      char c = (char)BTSerial.read();
      // 줄바꿈 문자를 기준으로 명령어를 구분한다고 가정
      if (c == '\n' || c == '\r')
        break;
      command += c;
    }   

    command.trim();

    // 받은 명령 확인 (시리얼 모니터에 출력)
    Serial.print("Received command: ");
    Serial.println(command);

    if (command == "4") 
    {
      Serial.println("LED 키기");
      LEDOn();  
    } 
    else if (command == "5") 
    {
      Serial.println("LED 끄기");
      LEDOff();  
    }
    else 
    {
      Serial.println("Invalid command received.");
    }
  }
}

void LEDOn()
{
  analogWrite(RED_PIN, 0);
  analogWrite(GREEN_PIN, 0);
  analogWrite(BLUE_PIN, 0);
}

void LEDOff()
{
  analogWrite(RED_PIN, 255);
  analogWrite(GREEN_PIN, 255);
  analogWrite(BLUE_PIN, 255);
}