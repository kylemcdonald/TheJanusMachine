const int buttonPin = A0;
const int LEDPin = 11;
const int ringLightPin = 10;
int ledSwitch = 0;

const byte LEDHigh = 255 - 64;
const byte LEDLow = 0;
const byte ringLightHigh = 255 - 16;
const byte ringLightLow = 128 + 8;

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
    analogWrite(LEDPin, map(ledSwitch, 0, 255, LEDLow, LEDHigh));
    analogWrite(ringLightPin, map(ledSwitch, 0, 255, ringLightHigh, ringLightLow));
  }

  delay(30);
}
