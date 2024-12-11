#include <Servo.h>       // 서보모터 라이브러리
#include <DHT.h>         // DHT 센서 라이브러리
#include <SoftwareSerial.h> // 블루투스 모듈 라이브러리
#define DHTTYPE DHT11    // 센서 유형 (DHT11 또는 DHT22)

// BLUE_PINtooth
SoftwareSerial BTSerial(10,11); // 2(TX),3(RX)

// 온습도 센서
int DHT_PIN = 3;         // DHT 센서 데이터 핀 (예: D3 핀에 연결)
DHT dht(DHT_PIN, DHTTYPE); // DHT 센서 객체 생성

/*
인체 감지 센서를 사용한 출입 감지 기능 제공
센서 1 -> 센서 2 : 들어오는 경우
센서 2 -> 센서 1 : 나가는 경우

각 Digital pin 4, 5 사용
*/
int HUMAN_1_PIN = 4; // 센서 1
int HUMAN_2_PIN = 5; // 센서 2

int state_in_1 = 0; // 센서 1 상태
int state_in_2 = 0; // 센서 2 상태

unsigned long lastEventTime = 0; // 마지막 이벤트 시간
const unsigned long debounceDelay = 500; // 디바운스 시간(ms), 0.5s

// LED Pin
int RED_PIN = 6;
int GREEN_PIN = 7;
int BLUE_PIN = 8;

// Motor
Servo myServo;           // 서보 객체 생성
int MOTOR_PIN = 9;      // 서보 모터 9번 핀
int MAX_MOTOR = 120;
int MIN_MOTOR = 0;

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

  // Motor
  dht.begin();           // DHT 센서 초기화
  myServo.attach(9);     // 서보모터를 9번 핀에 연결
  deactivateMotor();

  // Human Sensing
  pinMode(HUMAN_1_PIN, INPUT);
  pinMode(HUMAN_2_PIN, INPUT);
}

// LED 및 창 상태 정의
char* LEDState = "ON";      // LED 상태 (ON/OFF)
char* WindowState = "OPEN";  // 창 상태 (OPEN/CLOSED)

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
      WindowState = "OPEN";  // 창 상태 (OPEN/CLOSED)
      activateMotor();
      getTemperation();
    } 
    else if (command == "2") 
    {
      WindowState = "CLOSE";  // 창 상태 (OPEN/CLOSED)
      deactivateMotor();
      getTemperation();
    } 
    else if (command == "3")
    {
      LEDState = "ON";      // LED 상태 (ON/OFF)
      LEDOn();
      getTemperation();
    }
    else if (command == "4")
    {
      LEDState = "OFF";      // LED 상태 (ON/OFF)      
      LEDOff();
      getTemperation();
    }
    else if (command == "5")
    {
      getTemperation();
    }
    else 
    {
      Serial.println("Invalid command received.");
    }
  }
}


void sensingHuman()
{
  // 센서 상태 읽기
  int humanRead1 = digitalRead(HUMAN_1_PIN);
  int humanRead2 = digitalRead(HUMAN_2_PIN);

  // 현재 시간 확인
  unsigned long currentTime = millis();

  // 이벤트 처리

  // 1. 센서 2 -> 센서 1 : 나가는 경우
  if (isSensorActivated(humanRead1, state_in_1, currentTime)) 
  {
    state_in_1 = 1; // 센서 1 활성화
    if (state_in_2 == 1) 
    { // 센서 2가 이미 활성화된 경우
      Serial.println("사람이 나감");
      resetStates();
    }
    lastEventTime = currentTime; // 이벤트 시간 갱신
  }

  // 2. 센서 1 -> 센서 2 : 나가는 경우
  if (isSensorActivated(humanRead2, state_in_2, currentTime)) 
  {
    state_in_2 = 1; // 센서 2 활성화
    if (state_in_1 == 1) 
    { // 센서 1이 이미 활성화된 경우
      Serial.println("사람이 들어옴");
      resetStates();
    }
    lastEventTime = currentTime; // 이벤트 시간 갱신
  }

  // 센서 비활성화 확인 (0으로 돌아오는지 확인)
  if (humanRead1 == LOW && state_in_1 == 1) 
  {
    state_in_1 = 0;
  }

  if (humanRead2 == LOW && state_in_2 == 1) 
  {
    state_in_2 = 0;
  }
}

void resetStates() 
{
  state_in_1 = 0;
  state_in_2 = 0;
}

// 센서 활성화 조건 확인
bool isSensorActivated(int readPin, int state, unsigned long currentTime) 
{
  return (readPin == HIGH && state == 0 && (currentTime - lastEventTime > debounceDelay));
}

void LEDOn()
{
  Serial.println("LED 켜기");

  analogWrite(RED_PIN, 0);
  analogWrite(GREEN_PIN, 0);
  analogWrite(BLUE_PIN, 0);
}

void LEDOff()
{
  Serial.println("LED 끄기");

  analogWrite(RED_PIN, 255);
  analogWrite(GREEN_PIN, 255);
  analogWrite(BLUE_PIN, 255);
}

void getTemperation()
{
  Serial.println("온습도 가져오기");
  
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
  Serial.println("창문 열기");
  myServo.write(MAX_MOTOR); // 서보모터를 60도로 이동
}

void deactivateMotor()
{
  Serial.println("창문 닫기");
  myServo.write(MIN_MOTOR); // 서보모터를 60도로 이동
}