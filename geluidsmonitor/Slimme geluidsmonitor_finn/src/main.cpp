#include <Arduino.h>

const int micPin = A0; // Pin A0 is used for the microphone input

int sample; // Variable to store the current sound sample
int maxVal = 0; // Variable to track the maximum sound level detected
int minVal = 1023; // Variable to track the minimum sound level detected

// Shift register pins
int LatchPin = 3; // Pin 3 is connected to the LatchPin (ST_CP) of the 74HC595
int ClockPin = 5; // Pin 5 is connected to the ClockPin (SH_CP) of the 74HC595
int DataPin = 2; // Pin 2 is connected to the DataPin (DS) of the 74HC595

// Binary patterns for the LEDs, representing different sound levels
byte LEDs0 = 0b10000000; // LED pattern for the lowest sound level
byte LEDs1 = 0b10000001;
byte LEDs2 = 0b10000011; 
byte LEDs3 = 0b10000111; 
byte LEDs4 = 0b10001111; 
byte LEDs5 = 0b10011111; 
byte LEDs6 = 0b10111111; 
byte LEDs7 = 0b11111111; // LED pattern for the highest sound level

void setup() {
  pinMode(LatchPin, OUTPUT);
  pinMode(ClockPin, OUTPUT);
  pinMode(DataPin, OUTPUT);
  pinMode(micPin, INPUT);
  Serial.begin(9600);
}

void loop() {
  for (int i = 0; i < 100; i++) {
    sample = analogRead(micPin);
    if (sample > maxVal) maxVal = sample;
    if (sample < minVal) minVal = sample;
  }

  int peakToPeak = maxVal - minVal;

  digitalWrite(LatchPin, LOW);

  // Thresholds nu direct op peakToPeak, gebaseerd op je gemeten bereik (2-24)
  if (peakToPeak > 18) {
    shiftOut(DataPin, ClockPin, LSBFIRST, LEDs7);
  }
  else if (peakToPeak > 14) {
    shiftOut(DataPin, ClockPin, LSBFIRST, LEDs6);
  }
  else if (peakToPeak > 10) {
    shiftOut(DataPin, ClockPin, LSBFIRST, LEDs5);
  }
  else if (peakToPeak > 7) {
    shiftOut(DataPin, ClockPin, LSBFIRST, LEDs4);
  }
  else if (peakToPeak > 5) {
    shiftOut(DataPin, ClockPin, LSBFIRST, LEDs3);
  }
  else if (peakToPeak > 3) {
    shiftOut(DataPin, ClockPin, LSBFIRST, LEDs2);
  }
  else if (peakToPeak > 2) {
    shiftOut(DataPin, ClockPin, LSBFIRST, LEDs1);
  }
  else {
    shiftOut(DataPin, ClockPin, LSBFIRST, LEDs0);
  }
  digitalWrite(LatchPin, HIGH);

  maxVal = 0;
  minVal = 1023;

  // Geen spaties na de dubbele punt, en voltage weggehaald (voegde niks toe)
  Serial.print(">");
  Serial.print("peakToPeak:");
  Serial.println(peakToPeak);

  delay(100);
}