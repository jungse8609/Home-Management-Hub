/*
인체 감지 센서를 사용한 출입 감지 기능 제공
센서 1 -> 센서 2 : 들어오는 경우
센서 2 -> 센서 1 : 나가는 경우

각 Digital pin 7, 8 사용
*/
int pin_in_1 = 7; // 센서 1
int pin_in_2 = 8; // 센서 2

int readPin1;
int readPin2;

int state_in_1 = 0; // 센서 1 상태
int state_in_2 = 0; // 센서 2 상태

unsigned long lastEventTime = 0; // 마지막 이벤트 시간
const unsigned long debounceDelay = 500; // 디바운스 시간(ms), 0.5s

void setup() {
  Serial.begin(9600);
  pinMode(pin_in_1, INPUT);
  pinMode(pin_in_2, INPUT);
}

void loop() {
  // 센서 상태 읽기
  readPin1 = digitalRead(pin_in_1);
  readPin2 = digitalRead(pin_in_2);

  // 현재 시간 확인
  unsigned long currentTime = millis();

  // 이벤트 처리

  // 1. 센서 2 -> 센서 1 : 나가는 경우
  if (isSensorActivated(readPin1, state_in_1, currentTime)) 
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
  if (isSensorActivated(readPin2, state_in_2, currentTime)) 
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
  if (readPin1 == LOW && state_in_1 == 1) 
  {
    state_in_1 = 0;
  }

  if (readPin2 == LOW && state_in_2 == 1) 
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
  return (readPin == HIGH && state == 0 && isDebounced(currentTime));
}
