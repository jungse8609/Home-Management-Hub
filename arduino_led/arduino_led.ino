int RED = 11;
int GREEN = 10;
int BLUE = 9;
  
void setup()
{
  pinMode(RED, OUTPUT); 
  pinMode(GREEN, OUTPUT);
  pinMode(BLUE, OUTPUT);  
}
  
void loop()
{
  analogWrite(RED, 255); // RED ON
  analogWrite(GREEN, 0);
  analogWrite(BLUE, 0);
  delay(1000);
   
  analogWrite(RED, 0);
  analogWrite(GREEN, 255); // GREEN ON
  analogWrite(BLUE, 0);  
  delay(1000);
 
  analogWrite(RED, 0);
  analogWrite(GREEN, 0);
  analogWrite(BLUE, 255);    // BLUE ON
  delay(1000);
}