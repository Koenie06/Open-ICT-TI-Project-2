#include <Arduino.h>

int overlast = 0;
int kalmte = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  int analogValue = analogRead(0); // Measure the audio sensor input
  long sum = 0;
  const int samples = 200;

  // Find average (DC level)
  for (int i = 0; i < samples; i++) {
    sum += analogRead(0);
  }

  float average = sum / (float)samples;

  // Calculate signal amplitude
  long sumSquares = 0;

  for (int i = 0; i < samples; i++) {
    float x = analogRead(0) - average;
    sumSquares += x * x;
  }

  float rms = sqrt(sumSquares / (float)samples);

  Serial.println(rms);

  if ((rms > 2.59) && (kalmte < 1) && (overlast < 12)) {
    overlast = overlast + 1;
  }
  else if ((rms < 2.50) && (overlast > 0)) {
    overlast = overlast - 1;
  }

if ((overlast > 9) && (kalmte < 0)) {
    digitalWrite(2, LOW);
    digitalWrite(3, LOW);
    digitalWrite(4, HIGH);
    digitalWrite(5, HIGH);
    delay(500);
    digitalWrite(5, LOW);
  }
else if (kalmte < 1) {
  digitalWrite (2, LOW);
  digitalWrite(3, HIGH);
  digitalWrite(4, LOW);
  };

if ((rms < 0.1) && (overlast == 0)) {
    digitalWrite(2, HIGH);
    digitalWrite(3, LOW);
    digitalWrite(4, LOW);
    if(kalmte < 5){
    kalmte = kalmte + 1;
    };
}
else if (kalmte > -3) {
    kalmte = kalmte - 1;
};

if (kalmte > 0) {
    digitalWrite(2, HIGH);
    digitalWrite(3, LOW);
    digitalWrite(4, LOW);
};

/* // Debuging (optional)
Serial.println("Analog:");
Serial.println(analogValue);
Serial.println("Overlast:");
Serial.println(overlast);
Serial.println("Kalmte:");
Serial.println(kalmte);
// End Debug
*/
delay(50);
}