/* Alarm system using UNO R3 controller, HC-SR501 motion sensor, 4x4 keypad , Active buzzer , 1 red led and 1 green led
When motion is detected by the sensor an Alarm lighting up the red led and switching on the buzzer until the correct 4 digit password is inputed into the keypad
If inputed password is incorrect then red led will flash and user will be required to re-input a new password
Created by Justin Huang
*/
#include <Keypad.h>

const byte ROWS = 4; 
const byte COLS = 4; 
int pir = 10; // input from Motion sensor
int ledr = 11; // RED LED
int ledg = 12; // GREEN LED
int alarm = 13; // Active Buzzer
int pirValue; 
bool clear = true;   // Successfully turning off the alarm will set this to true

char hexaKeys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte rowPins[ROWS] = {9, 8, 7, 6}; 
byte colPins[COLS] = {5, 4, 3, 2}; 

Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 

void setup() {
  Serial.begin(9600);
  pinMode(pir, INPUT);
  pinMode(alarm, OUTPUT);
  pinMode(ledr, OUTPUT);
  pinMode(ledg, OUTPUT);

  digitalWrite(pir, LOW);
  digitalWrite(ledr, LOW);
  digitalWrite(ledg, HIGH);
}

void loop() {
  char password[4] = {'1', '2', '3', '4'}; // Correct password sequence
  char input[4] = {}; 
  int motion = digitalRead(pir);

  if (motion) {
    digitalWrite(ledg, LOW);
    digitalWrite(ledr, HIGH);
    digitalWrite(alarm, HIGH);
    clear = false;
    while (!clear) { 
      int index = 0;
      Serial.println("Alarm is activated!");
      input[4] = {};
        while (index < 4) { 
          char inputKey = customKeypad.getKey(); // Continuously check for keypad input
            if (inputKey) { // Only proceed if a key is pressed
              input[index] = inputKey;
              index++;
            }
          }
      // Check if the input matches the password
      bool isCorrect = true;
      for (int j = 0; j < 4; j++) {
        if (input[j] != password[j]) {
          isCorrect = false;
          break;
        }
      }

      if (isCorrect) {
        Serial.println("You have entered the corect password");
        digitalWrite(ledr, LOW);
        digitalWrite(ledg, HIGH);
        digitalWrite(alarm, LOW);
        clear = true;
      } else {
        Serial.println("You have entered the incorect password");
        // Flash the red LED for incorrect password
        for (int k = 0; k < 6;k++) {
          digitalWrite(ledr, HIGH);
          delay(300);
          digitalWrite(ledr, LOW);
          delay(300);
          digitalWrite(ledr,HIGH);
        }
      }
    }
  }
}


