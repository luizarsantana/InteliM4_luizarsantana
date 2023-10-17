void setup() {
  pinMode(4, OUTPUT);
}

void loop() {
  digitalWrite(4, HIGH);
  delay(600); 
  digitalWrite(4, LOW);
  delay(600); 
}