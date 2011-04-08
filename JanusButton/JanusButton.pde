const int buttonPin = A0;
const int LEDPin = 11;
const int ringLightPin = 10;

const byte LEDHigh = 255 - 64;
const byte LEDLow = 0;
const byte ringLightHigh = 255 - 16;
const byte ringLightLow = 128 + 8;

boolean previousState = LOW;
int previousFadeState = 0;

void setup(){
  pinMode(LEDPin, OUTPUT);
  pinMode(buttonPin, INPUT);
  digitalWrite(buttonPin, HIGH);
  Serial.begin(9600);
}

float getElapsedTimef() {
  return millis() / 1000.f;
}

void loop() {
   boolean currentState = digitalRead(buttonPin);
  
  if (currentState == LOW && previousState == HIGH){
    Serial.print("1"); 
  }
  previousState = currentState;
  
  while (Serial.available()) {
    int fadeState = Serial.read();
    float pulse = map(sin(getElapsedTimef() * 2), -1, 1, 0, 1);
    analogWrite(LEDPin, map(pulse * fadeState, 0, 255, LEDLow, LEDHigh));
    analogWrite(ringLightPin, map(fadeState, 0, 255, ringLightHigh, ringLightLow));
    previousFadeState = fadeState;
  }

  delay(5);
}
