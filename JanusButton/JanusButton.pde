const int buttonPin = A0;
const int LEDPin = 11;
int ledSwitch = 0;

boolean previousState = false;

void setup(){
  pinMode(LEDPin, OUTPUT);
  pinMode(buttonPin, INPUT);
  digitalWrite(buttonPin, HIGH);
  Serial.begin(9600);
}

void loop() {
   boolean currentState = digitalRead(buttonPin);
  
  if (currentState == LOW && previousState == HIGH){
    Serial.print("1"); 
  }
  previousState = currentState;
  
  while (Serial.available()) {
    ledSwitch = Serial.read();
    analogWrite(LEDPin, ledSwitch);
  }

  delay(30);
}
