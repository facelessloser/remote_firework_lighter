
#include <IRremote.h>

const int buttonPin = 13; // Pushbutton on pin 13
const int Relay = 8; // Relay on pin 8

// The remote button for on and off

#define button1 2080
#define button2 33

// The IR stuff

int RECV_PIN = 11;
IRrecv irrecv(RECV_PIN);
long lReceived = 0 ;
decode_results results;

// Some unsigned long for the timings

unsigned long currentTime;
unsigned long loopTime1;
unsigned long loopTime2;

// LED's

int ledGreen = 3; // Green LED
int ledRed = 6; // Red LED

// Buttons

int buttonState;             // the current reading from the input pin
int lastButtonState = LOW;   // the previous reading from the input pin

// Debounce

long lastDebounceTime = 0;  // the last time the output pin was toggled
long debounceDelay = 50;    // the debounce time; increase if the output flickers

void setup() {
  pinMode(ledGreen, OUTPUT);
  pinMode(ledRed, OUTPUT);
  pinMode(buttonPin, INPUT);
  pinMode(Relay, OUTPUT);
  irrecv.enableIRIn(); // Start the receiver
  Serial.begin(9600);
}

void dump(decode_results *results) {
  int count = results->rawlen;
}

void loop() {
  currentTime = millis();
  loopTime1;
  loopTime2;

// IR code

  if (irrecv.decode(&results)) {
    lReceived = results.value ;
    Serial.println(results.value);
    switch (lReceived) {
      case button1:
        digitalWrite(Relay, LOW);
        digitalWrite(ledRed, HIGH);
        break;
      case button2:
        digitalWrite(Relay, HIGH);
        digitalWrite(ledRed, LOW);
        break;
    }
    dump(&results);
    irrecv.resume();
  }

  int reading = digitalRead(buttonPin);
  
  // Turns the relay on
  if (currentTime == loopTime1) {
    digitalWrite(ledRed, HIGH);
    digitalWrite(ledGreen, LOW);
    digitalWrite(Relay, LOW);
  }
  // Turn the relay off
  if (currentTime == loopTime2) {
    digitalWrite(ledRed, LOW);
    digitalWrite(ledGreen, LOW);
    digitalWrite(Relay, HIGH);
    }
    
  if (reading != lastButtonState) {
    // reset the debouncing timer
    lastDebounceTime = millis();
  } 
  
  if ((millis() - lastDebounceTime) > debounceDelay) {

    if (reading != buttonState) {
      buttonState = reading;

      if (buttonState == LOW) {
        digitalWrite(ledRed, LOW);
        digitalWrite(ledGreen, HIGH);
        digitalWrite(Relay, HIGH);
        // This is the run away time
        loopTime1 = currentTime += 5000;
        // This is the bur
        loopTime2 = currentTime += 5000;
      }
    }
  }

  lastButtonState = reading;
}

