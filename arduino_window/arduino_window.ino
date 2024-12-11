#include <Servo.h>       // 서보모터 라이브러리
#include <DHT.h>         // DHT 센서 라이브러리
#include <SoftwareSerial.h> // 블루투스 모듈 라이브러리
#define DHTTYPE DHT11    // 센서 유형 (DHT11 또는 DHT22)

// BLUE_PINtooth
SoftwareSerial BTSerial(10,11); // 2(TX),3(RX)

int DHT_PIN = 3;         // DHT 센서 데이터 핀 (예: D3 핀에 연결)
DHT dht(DHT_PIN, DHTTYPE); // DHT 센서 객체 생성

// Motor
Servo myServo;           // 서보 객체 생성
int MOTOR_PIN = 9;      // 서보 모터 9번 핀
int MAX_MOTOR = 60;
int MIN_MOTOR = 0;

void setup() 
{
  Serial.begin(9600);     // PC 시리얼 모니터 통신
  BTSerial.begin(9600);   // HC-06 블루투스 모듈 통신 속도 설정

  // Motor
  dht.begin();           // DHT 센서 초기화
  myServo.attach(9);     // 서보모터를 9번 핀에 연결
  deactivateMotor();
}

// LED 및 창 상태 정의
char* LEDState = "ON";      // LED 상태 (ON/OFF)
char* WindowState = "OFF";  // 창 상태 (OPEN/CLOSED)

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

    if (command == "1") 
    {
      activateMotor();
    } 
    else if (command == "2") 
    {
      deactivateMotor();
    } 
    else if (command == "3")
    {
      Serial.println("온습도 가져오기");
      getTemperation();
    }
    else if (command == "4")
    {
      Serial.println("LED 바꾸기 @@@@@@@@@@");
      LEDState = "OFF";      // LED 상태 (ON/OFF)
      WindowState = "ON";  // 창 상태 (OPEN/CLOSED)
      getTemperation();
    }
    else if (command == "5")
    {
      Serial.println("LED 바꾸기 ##########");
      LEDState = "ON";      // LED 상태 (ON/OFF)
      WindowState = "OFF";  // 창 상태 (OPEN/CLOSED)
      getTemperation();
    }
    else 
    {
      Serial.println("Invalid command received.");
    }
  }
}

void getTemperation()
{
  // 온습도 데이터 읽기
  float temperature = dht.readTemperature(); // 섭씨 온도 읽기
  float humidity = dht.readHumidity();       // 습도 읽기

  // 포맷된 문자열을 저장할 버퍼
  char formattedData[100];
  char tempBuffer[10], humBuffer[10];

  // float 값을 문자열로 변환
  dtostrf(temperature, 6, 2, tempBuffer);  // 6자리, 소수점 이하 2자리
  dtostrf(humidity, 6, 2, humBuffer);

  // 문자열 포맷팅
  sprintf(formattedData, "T:%s H:%s LED:%s WIN:%s",
        tempBuffer,
        humBuffer,
        LEDState,
        WindowState);

  // 딜레이 하기
  delay(500);

  // 블루투스 통신으로 데이터 전송
  BTSerial.println(formattedData);  // 개행 포함

  Serial.println(formattedData);
}

void activateMotor()
{
  Serial.println("모터 가동");
  myServo.write(MAX_MOTOR); // 서보모터를 60도로 이동
}

void deactivateMotor()
{
  Serial.println("모터 원위치");
  myServo.write(MIN_MOTOR); // 서보모터를 60도로 이동
}