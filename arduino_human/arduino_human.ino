#include <SoftwareSerial.h> // 블루투스 모듈 라이브러리

// BLUE_PINtooth
SoftwareSerial BTSerial(10,11); // 10(TX),11(RX)

/*
인체 감지 센서를 사용한 출입 감지 기능 제공
센서 1 -> 센서 2 : 들어오는 경우
센서 2 -> 센서 1 : 나가는 경우
*/
int HUMAN_1_PIN = 4; // 센서 1 PIN 4
int HUMAN_2_PIN = 5; // 센서 2 PIN 5

int state_in_1 = 0; // 센서 1 상태
int state_in_2 = 0; // 센서 2 상태

unsigned long lastEventTime = 0; // 마지막 이벤트 시간
const unsigned long debounceDelay = 500; // 디바운스 시간(ms), 0.5s

void setup() {
  Serial.begin(9600);     // PC 시리얼 모니터 통신
  BTSerial.begin(9600);   // HC-06 블루투스 모듈 통신 속도 설정

  // Human Sensing
  pinMode(HUMAN_1_PIN, INPUT);
  pinMode(HUMAN_2_PIN, INPUT);
}

void loop() 
{
  int humanRead1 = digitalRead(HUMAN_1_PIN);
  Serial.println(humanRead1);
  delay(500);

  if(Serial.available()) // 시리얼 통신으로 문자가 들어오면
  {
    Serial.read(); // 블루투스시리얼 통신으로 발송
    
    char buffer[10];
    itoa(humanRead1, buffer, 10);
    BTSerial.write(buffer); // 블루투스시리얼 통신으로 발송
  }
  if(BTSerial.available()) // 블루투스 시리얼 통신으로 문자가 들어오면
  {
    Serial.write(Serial.read()); // 블루투스시리얼 통신으로 발송
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