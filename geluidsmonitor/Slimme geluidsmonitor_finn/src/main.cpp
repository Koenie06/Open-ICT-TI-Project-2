#include <Arduino.h>

int overlast = 0;
int kalmte = 0;

const int samples = 300;
const int deltaThreshold = 50; // pas dit aan op basis van wat je in Serial Monitor ziet

void setup() {
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  int mn = 1024;
  int mx = 0;

  for (int i = 0; i < samples; ++i) {
    int val = analogRead(0);
    mn = min(mn, val);
    mx = max(mx, val);
  }

  int delta = mx - mn;

  Serial.print("Min=");
  Serial.print(mn);
  Serial.print(" Max=");
  Serial.print(mx);
  Serial.print(" Delta=");
  Serial.println(delta);

  // DE ONTBREKENDE SCHAKEL: delta daadwerkelijk gebruiken om overlast bij te werken
  if ((delta > deltaThreshold) && (kalmte < 1) && (overlast < 12)) {
    overlast = overlast + 1;
  }
  else if ((delta < deltaThreshold / 2) && (overlast > 0)) {
    overlast = overlast - 1;
  }

  if ((overlast > 9) && (kalmte < 0)) {
    digitalWrite(2, LOW);
    digitalWrite(3, LOW);
    digitalWrite(4, HIGH);
    digitalWrite(5, HIGH);
    digitalWrite(5, LOW);
  }
  else if (kalmte < 1) {
    digitalWrite(2, LOW);
    digitalWrite(3, HIGH);
    digitalWrite(4, LOW);
  }

  if (overlast == 0) {
    digitalWrite(2, HIGH);
    digitalWrite(3, LOW);
    digitalWrite(4, LOW);
    if (kalmte < 5) {
      kalmte = kalmte + 1;
    }
  }
  else if (kalmte > -3) {
    kalmte = kalmte - 1;
  }

  if (kalmte > 0) {
    digitalWrite(2, HIGH);
    digitalWrite(3, LOW);
    digitalWrite(4, LOW);
  }

  delay(50);
}