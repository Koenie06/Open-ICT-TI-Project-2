#include <Arduino.h>

int overlast = 0;
int kalmte = 0;

const int SENSOR_PIN = A0; // Pin for the sound sensor (Analog pin A0)
const int PIN_QUIET = 2;   // Pin for "Quiet" level output
const int PIN_MODERATE = 3; // Pin for "Moderate" level output
const int PIN_LOUD = 4;    // Pin for "Loud" level output
const int PIN_ALARM = 5; //alarm bij te loud

const int sampleWindow = 50; // Sample window size in milliseconds (50 ms = 20 Hz)
unsigned int sample;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600); // Start serial communication for debugging

  pinMode(SENSOR_PIN, INPUT); // Set the sound sensor pin as an input
  pinMode(PIN_QUIET, OUTPUT);  // Set the pins for sound level indicators as output
  pinMode(PIN_MODERATE, OUTPUT);
  pinMode(PIN_LOUD, OUTPUT);
  pinMode(PIN_ALARM,OUTPUT);
  
  // Set the sound level output pins to LOW initially
  digitalWrite(PIN_QUIET, LOW);
  digitalWrite(PIN_MODERATE, LOW);
  digitalWrite(PIN_LOUD, LOW);
  digitalWrite(PIN_ALARM,LOW);
  
}

void loop() {

  unsigned long startMillis = millis(); // Start timing for the sample window
  float peakToPeak = 0;  // Variable to store the peak-to-peak value
  unsigned int signalMax = 0; // Maximum signal value during the window
  unsigned int signalMin = 1024; // Minimum signal value during the window
  
  // Collect data for the duration of the sample window (50 ms)
  while (millis() - startMillis < sampleWindow) {
    sample = analogRead(SENSOR_PIN); // Read the value from the sound sensor

    // Filter out invalid values
    if (sample < 1024) {
      if (sample > signalMax) {
        signalMax = sample; // Update the maximum value
      } else if (sample < signalMin) {
        signalMin = sample; // Update the minimum value
      }
    }
  }

  peakToPeak = signalMax - signalMin; // Calculate the peak-to-peak amplitude
  int db = map(peakToPeak, 20, 750, 49.5, 90); // Map the amplitude to decibels (dB)
  

  // Set the output level based on the dB value
  if (db <= 60) {
    digitalWrite(PIN_QUIET, HIGH); // Activate Quiet level
    digitalWrite(PIN_MODERATE, LOW); // Deactivate Moderate level
    digitalWrite(PIN_LOUD, LOW);    // Deactivate Loud level
  } else if (db > 60 && db < 85) {
    digitalWrite(PIN_QUIET, LOW);   // Deactivate Quiet level
    digitalWrite(PIN_MODERATE, HIGH); // Activate Moderate level
    digitalWrite(PIN_LOUD, LOW);    // Deactivate Loud level
  } else if (db >= 85) {
    digitalWrite(PIN_ALARM, HIGH);
    digitalWrite(PIN_QUIET, LOW);   // Deactivate Quiet level
    digitalWrite(PIN_MODERATE, LOW); // Deactivate Moderate level
    digitalWrite(PIN_LOUD, HIGH);   // Activate Loud level
  }

  Serial.println(db);

  delay(500); // Wait for 1.5 seconds before the next reading

/* // Debuging (optional)
Serial.println("Analog:");
Serial.println(analogValue);
Serial.println("Overlast:");
Serial.println(overlast);
Serial.println("Kalmte:");
Serial.println(kalmte);
// End Debug
*/

}