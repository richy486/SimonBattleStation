/*
  Button

 Turns on and off a light emitting diode(LED) connected to digital
 pin 13, when pressing a pushbutton attached to pin 2.


 The circuit:
 * LED attached from pin 13 to ground
 * pushbutton attached to pin 2 from +5V
 * 10K resistor attached to pin 2 from ground

 * Note: on most Arduinos there is already an LED on the board
 attached to pin 13.


 created 2005
 by DojoDave <http://www.0j0.org>
 modified 30 Aug 2011
 by Tom Igoe

 This example code is in the public domain.

 http://www.arduino.cc/en/Tutorial/Button
 */

// constants won't change. They're used here to
// set pin numbers:
//const int buttonPin = 2;     // the number of the pushbutton pin
const int ledPin =  13;      // the number of the LED pin

const int BUTTONS = 5;
const int buttonPins[] = {2, 3, 4, 5, 6};
const int LEDs = 4;
const int ledPins[] = {8, 9, 10, 11};

// variables will change:
int buttonState = 0;         // variable for reading the pushbutton status
int states[BUTTONS];

void setup() {
  Serial.begin(9600);
  // initialize the LED pin as an output:
  pinMode(ledPin, OUTPUT);
  // initialize the pushbutton pin as an input:
//  pinMode(buttonPin, INPUT);

//  for (int i = 0; i < BUTTONS; i += 1) {
//    pinMode(buttonPins[i], INPUT);
//  }

    pinMode(2, INPUT);
    pinMode(3, INPUT);
    pinMode(4, INPUT);
    pinMode(5, INPUT);
    pinMode(6, INPUT);

//  for (int i = BUTTONS; i < 13; i += 1) {
//    pinMode(buttonPins[i] + BUTTONS, OUTPUT);
//  }

  for (int i = 0; i < LEDs; i += 1) {
    pinMode(ledPins[i], OUTPUT);
  }

  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);
}

void loop() {
  // read the state of the pushbutton value:
  buttonState = LOW;

  

//  Serial.print("------------\n");
//  for (int i = 0; i < BUTTONS; i += 1) {
//
//    Serial.print(" ");
//    Serial.print(buttonPins[i]);
//  }
//  Serial.print("\n");
  
  for (int i = 0; i < BUTTONS; i += 1) {

    states[i] = digitalRead(buttonPins[i]);

    if (states[i] == HIGH) {
      buttonState = HIGH;
    }

    Serial.print(" ");
    Serial.print(states[i]);
  }

  int value = analogRead(A1); 
  Serial.print(", analogue val: ");
  Serial.print(value);
  

  
  Serial.print("\n");

  for (int i = 0; i < LEDs; i += 1) {

    if (states[i] == HIGH) {
      digitalWrite(ledPins[i], HIGH);
    } else {
      digitalWrite(ledPins[i], LOW);
    }
    
  }

//  digitalWrite(8, HIGH);
//  digitalWrite(9, HIGH);
//  digitalWrite(10, HIGH);
//  digitalWrite(11, HIGH);
//  digitalWrite(12, HIGH);
  

  // check if the pushbutton is pressed.
  // if it is, the buttonState is HIGH:
  if (buttonState == HIGH) {
    // turn LED on:
    
    digitalWrite(ledPin, HIGH);
  } else {
    // turn LED off:
    digitalWrite(ledPin, LOW);
  }
}
