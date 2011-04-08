const int buttonPin = A0;
const int LEDPin = 11;
const int ringLightPin = 10;

const byte LEDHigh = 255 - 64;
const byte LEDLow = 0;
const byte ringLightHigh = 255 - 16;
const byte ringLightLow = 128 + 8;

boolean previousState = LOW;
int previousFadeState = 0;
int fadeState;

void setup(){
  pinMode(LEDPin, OUTPUT);
  pinMode(buttonPin, INPUT);
  digitalWrite(buttonPin, HIGH);
  Serial.begin(9600);
}

float getElapsedTimef() {
  return millis() / 1000.0f;
}

void loop() {
  boolean currentState = digitalRead(buttonPin);

  if (currentState == LOW && previousState == HIGH){
    Serial.print("1"); 
  }
  previousState = currentState;

  while (Serial.available()) {
    fadeState = Serial.read();
    analogWrite(ringLightPin, map(fadeState, 0, 255, ringLightLow, ringLightHigh));
    previousFadeState = fadeState;
  }
  
  if(fadeState == 0) {
    int LEDRange = LEDHigh - LEDLow;
    analogWrite(LEDPin, (sin(getElapsedTimef() * 3) * .5 + .5) * LEDRange + LEDLow);
  } else {
    analogWrite(LEDPin, map(fadeState, 0, 255, LEDHigh, LEDLow));
  }
}

