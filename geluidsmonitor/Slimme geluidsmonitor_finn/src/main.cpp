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
  // Setting the shift register pins as outputs
  pinMode(LatchPin, OUTPUT);
  pinMode(ClockPin, OUTPUT);
  pinMode(DataPin, OUTPUT);

  // Setting the microphone pin as an input
  pinMode(micPin, INPUT);
  
  // Starting the serial communication for debugging (optional)
  Serial.begin(9600);
}

void loop() {
  // Process microphone data to determine the peak-to-peak value
  for (int i = 0; i < 100; i++) { // Take 100 samples to find the sound range
    sample = analogRead(micPin); // Read the current sound level from the microphone
    if (sample > maxVal) { // Update maxVal if the current sample is greater
      maxVal = sample;
    }
    if (sample < minVal) { // Update minVal if the current sample is lower
      minVal = sample;
    }
  }
  
  int peakToPeak = maxVal - minVal; // Calculate the peak-to-peak value

  // Convert the peak-to-peak value to a voltage level (0-5V range)
  float voltage = (peakToPeak * 5.0) / 1023.0;
  
  // Prepare to send data to the shift register by setting the latch pin low
  digitalWrite(LatchPin, LOW);
 
  // Determine which LED pattern to display based on the voltage level
  if(voltage > 2.4){ // Highest sound level
   shiftOut(DataPin, ClockPin, LSBFIRST, LEDs7); // Send the LED pattern for the highest sound
   digitalWrite(LatchPin, HIGH); // Latch the data to display the LEDs
  }
  else if(voltage > 2.1){ // Second highest sound level
   shiftOut(DataPin, ClockPin, LSBFIRST, LEDs6); // Send the corresponding LED pattern
   digitalWrite(LatchPin, HIGH); 
  }  
  else if(voltage > 1.8){ // Third highest sound level
   shiftOut(DataPin, ClockPin, LSBFIRST, LEDs5); // Send the corresponding LED pattern
   digitalWrite(LatchPin, HIGH); 
  }  
  else if(voltage > 1.5){ // Medium-high sound level
   shiftOut(DataPin, ClockPin, LSBFIRST, LEDs4); // Send the corresponding LED pattern
   digitalWrite(LatchPin, HIGH);     
  }  
 else if(voltage > 1.2){ // Medium sound level
   shiftOut(DataPin, ClockPin, LSBFIRST, LEDs3); // Send the corresponding LED pattern
   digitalWrite(LatchPin, HIGH);    
 }  
 else if(voltage > 0.9){ // Low-medium sound level
   shiftOut(DataPin, ClockPin, LSBFIRST, LEDs2); // Send the corresponding LED pattern
   digitalWrite(LatchPin, HIGH); 
 }  
 else if(voltage > 0.6){ // Low sound level
   shiftOut(DataPin, ClockPin, LSBFIRST, LEDs1); // Send the corresponding LED pattern
   digitalWrite(LatchPin, HIGH); 
 }  
 else if(voltage > 0.0){ // Lowest sound level
   shiftOut(DataPin, ClockPin, LSBFIRST, LEDs0); // Send the corresponding LED pattern
   digitalWrite(LatchPin, HIGH); 
 }   

  // Reset maxVal and minVal for the next set of samples
  maxVal = 0;
  minVal = 1023;
}