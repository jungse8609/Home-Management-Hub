int pin_in = 7;

void setup() {
  Serial.begin(9600);
  pinMode(pin_in, INPUT);
}

void loop() {
  int readPin = digitalRead(pin_in);
  Serial.println(readPin);
}
