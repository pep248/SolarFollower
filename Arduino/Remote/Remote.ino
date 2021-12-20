//Written by MrSirLRD
//code will pulse Led at pin 10 every 5mS
const int led = 10;
int dretes = 5;
int esquerres = 6;

void setup() {
  pinMode(led, OUTPUT);
  pinMode(dretes, INPUT);
  pinMode(esquerres, INPUT);
}

void loop() {
//dretes = digitalRead()
 // if (dretes == HIGH)
  //{
  digitalWrite(led, HIGH);
  delay(8);
  digitalWrite(led, LOW);
  delay(2);
  digitalWrite(led, HIGH);
  delay(4);
  digitalWrite(led, LOW);
  delay(2);
  digitalWrite(led, HIGH);
  delay(2);
  digitalWrite(led, LOW);
  delay(2);
  digitalWrite(led, HIGH);
  delay(2);
  digitalWrite(led, LOW);
  delay(2);
  digitalWrite(led, HIGH);
  delay(4);
  digitalWrite(led, LOW);
  delay(2);
  digitalWrite(led, HIGH);
  delay(2);
  digitalWrite(led, LOW);
  delay(2);
//  }
/*
//esquerres = digitalRead(6)
  if (esquerres == HIGH)
  {
  digitalWrite(led, HIGH);
  delay(8);
  digitalWrite(led, LOW);
  delay(2);
  digitalWrite(led, HIGH);
  delay(4);
  digitalWrite(led, LOW);
  delay(2);
  digitalWrite(led, HIGH);
  delay(2);
  digitalWrite(led, LOW);
  delay(2);
  digitalWrite(led, HIGH);
  delay(2);
  digitalWrite(led, LOW);
  delay(2);
  digitalWrite(led, HIGH);
  delay(4);
  digitalWrite(led, LOW);
  delay(2);
  digitalWrite(led, HIGH);
  delay(4);
  digitalWrite(led, LOW);
  delay(2);
  }*/
}
