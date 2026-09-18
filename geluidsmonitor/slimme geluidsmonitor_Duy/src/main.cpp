#include <Arduino.h>

// put function declarations here:
int myFunction(int, int);

//serial monitor is voor debuggen, zodat je kan zien wat er gebeurt in de code.
//er moeten 3 lampjes zijn, rood, geel en groen. Deze lampjes moeten op de juiste manier aan en uit gaan afhankelijk van de geluidsniveaus die worden gemeten door de geluidsmonitor.
// De geluidsniveaus worden gemeten door een microfoon die is aangesloten op de geluidsmonitor.
//bij een te luide omgeving voor een langere tijd gaat de active buzzer af en gaat het rode lampje aan. Bij een normale omgeving gaat het groene lampje aan. Bij een iets te luide omgeving gaat het gele lampje aan.
//het rode lampje moet 5 seconden lang branden voordat de buzzer afgaat.

// pin nummer voor de LEDjes, buzzer en microphone
const int greenLedPin = 2;    // Pin for green LED
const int yellowLedPin = 3; // Pin for yellow LED
const int redLedPin = 4;  // Pin for red LED
const int buzzerPin = 5;    // Pin for buzzer
const int micPin = A0;      // Pin for microphone

//variables voor de threshold 
const int redThreshold = 800;    // Threshold voor rode LED
const int yellowThreshold = 600; // Threshold voor gele LED
const unsigned long loudDuration = 5000; // tijd voordat de buzzer aangaat 

unsigned long loudSince = 0; // variable die meet hoelang het geluid te luid is
bool wasLoud = false; //variable om te kijken of het te luid is

// functie die het peak geluid leest over tijd en niet 1x per loop
int getPeakSoundLevel() {
  int peak = 0;
  unsigned long startTime = millis();
  while (millis() - startTime < 50) { // meet voor 50 millisecondes
    int reading = analogRead(micPin);
    if (reading > peak) {
      peak = reading;
    }
  }
  return peak;
}

//begin waardes
void setup() {
  // put your setup code here, to run once:
  pinMode(greenLedPin, OUTPUT);
  pinMode(yellowLedPin, OUTPUT);
  pinMode(redLedPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(micPin, INPUT);
  Serial.begin(9600); //debug methode via de terminal

}


void loop() {
  // put your main code here, to run repeatedly:
  int soundLevel = getPeakSoundLevel(); //leest de waardes van microphone
  Serial.println(soundLevel); //print de microphone waardes naar de terminal, serial

  //standaard zijn de leds uit
  digitalWrite(greenLedPin, LOW);
  digitalWrite(yellowLedPin, LOW);
  digitalWrite(redLedPin, LOW);

//Als het geluid hoger is dan rode threshold, rode lamp gaat aan, dan start de tijd en wanneer 5 secondes 
//zijn verstreken. dan gaat de buzzer aan
  if (soundLevel > redThreshold) {
    digitalWrite(redLedPin, HIGH);

    if (!wasLoud) {
      wasLoud = true;
      loudSince = millis(); //start de klok
    }
    
    if (millis() - loudSince >= loudDuration){
      digitalWrite(buzzerPin, HIGH); //pas na 5 sec luid
    } 
    else {
      digitalWrite(buzzerPin, LOW);
    }
  }
  
//Anders als het geluid hoger is dan gele threshold, dan gaat de gele lamp aan. Buzzer uit
  else if (soundLevel > yellowThreshold) {
    digitalWrite(yellowLedPin, HIGH);
    wasLoud = false;
    digitalWrite(buzzerPin, LOW);
  }

//Anders bij rust, gaat de groene lamp aan. Buzzer uit 
  else {
    digitalWrite(greenLedPin, HIGH);
    wasLoud = false;
    digitalWrite(buzzerPin, LOW);
  }
  }
