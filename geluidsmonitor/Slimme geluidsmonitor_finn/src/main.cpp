#include <Arduino.h>

// Pin nummers
const int greenLedPin = 2;
const int yellowLedPin = 3;
const int redLedPin = 4;
const int buzzerPin = 5;
const int micPin = A0;

const int samples = 200;

int overlast = 0;
int kalmte = 0;

// Non-blocking buzzer timer
unsigned long buzzerStart = 0;
bool buzzerActive = false;
const unsigned long buzzerDuration = 500; // hoe lang de buzzer piept

void setup() {
  pinMode(greenLedPin, OUTPUT);
  pinMode(yellowLedPin, OUTPUT);
  pinMode(redLedPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  // Metingen opslaan zodat we niet 2x apart hoeven te samplen
  int readings[samples];
  long sum = 0;

  for (int i = 0; i < samples; i++) {
    readings[i] = analogRead(micPin);
    sum += readings[i];
  }

  float average = sum / (float)samples;

  // RMS berekenen: hoeveel fluctueert het signaal rond zijn eigen gemiddelde
  long sumSquares = 0;
  for (int i = 0; i < samples; i++) {
    float x = readings[i] - average;
    sumSquares += x * x;
  }

  float rms = sqrt(sumSquares / (float)samples);
  Serial.println(rms);

  // Overlast-teller: telt op bij hard geluid, telt af bij rustig geluid
  if ((rms > 2.59) && (kalmte < 1) && (overlast < 12)) {
    overlast = overlast + 1;
  }
  else if ((rms < 2.50) && (overlast > 0)) {
    overlast = overlast - 1;
  }

  // TE DRUK: rode + gele LED, buzzer piept kort (non-blocking)
  if ((overlast > 9) && (kalmte < 0)) {
    digitalWrite(greenLedPin, LOW);
    digitalWrite(yellowLedPin, LOW);
    digitalWrite(redLedPin, HIGH);

    if (!buzzerActive) {
      digitalWrite(buzzerPin, HIGH);
      buzzerActive = true;
      buzzerStart = millis();
    }
  }
  else if (kalmte < 1) {
    digitalWrite(greenLedPin, LOW);
    digitalWrite(yellowLedPin, HIGH);
    digitalWrite(redLedPin, LOW);
  }

  // Buzzer automatisch uitzetten na buzzerDuration, ongeacht wat de rest van de loop doet
  if (buzzerActive && millis() - buzzerStart >= buzzerDuration) {
    digitalWrite(buzzerPin, LOW);
    buzzerActive = false;
  }

  // Kalmte-teller: telt op bij stilte, telt af bij niet-stilte
  if ((rms < 0.1) && (overlast == 0)) {
    digitalWrite(greenLedPin, HIGH);
    digitalWrite(yellowLedPin, LOW);
    digitalWrite(redLedPin, LOW);

    if (kalmte < 5) {
      kalmte = kalmte + 1;
    }
  }
  else if (kalmte > -3) {
    kalmte = kalmte - 1;
  }

  if (kalmte > 0) {
    digitalWrite(greenLedPin, HIGH);
    digitalWrite(yellowLedPin, LOW);
    digitalWrite(redLedPin, LOW);
  }

  /* // Debugging (optional)
  Serial.println("Overlast:");
  Serial.println(overlast);
  Serial.println("Kalmte:");
  Serial.println(kalmte);
  // End Debug
  */

  delay(50);
}