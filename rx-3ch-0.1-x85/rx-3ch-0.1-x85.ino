//----------------------------------------------------------------------------------------------
// RX part for rc car with authomatic stop lights (ATtiny85 based)
// cmd package format:
//    first byte:  0-99 steering level
//    second byte: 100-199 engine level
//----------------------------------------------------------------------------------------------
// SETTINGS
#define STEERING_MIN 30   // 0..90, 90 - center
#define STEERING_MAX 150  // 90..180, 90 - center
#define IS_REVERSE_STEERING false
#define IS_REVERSE_ENGINE false
#define STEERING_SMOOTH 5 // 1..n, n - more smooth
#define ENGINE_SMOOTH 10   // 1..n, n - more smooth 



#include <SoftwareServo.h>


#define LIGHT_PIN PIN_PB2
#define STEERING_PIN PIN_PB3
#define ENGINE_PIN PIN_PB4


SoftwareServo steeringServo;
SoftwareServo engineServo;
byte steeringCmd = 50;
byte engineCmd = 50;
byte lightCmd = 0;


void setup() {
  Serial.begin(9600);
  pinMode(LIGHT_PIN, OUTPUT);
  steeringServo.attach(STEERING_PIN);
  engineServo.attach(ENGINE_PIN);
  imAlive();
}


void loop() {
  if (Serial.available() > 0) {
    byte incomingByte = Serial.read();
    //Serial.println(incomingByte);
    if (incomingByte >= 0 && incomingByte <= 99) {           // steering
      steeringCmd = steeringCmd + ((incomingByte - steeringCmd) / STEERING_SMOOTH); //incomingByte;
    }
    else if (incomingByte >= 100 && incomingByte <= 199) {   // engine
      engineCmd = engineCmd + (((incomingByte - 100) - engineCmd) / ENGINE_SMOOTH); //incomingByte - 100;
    }
    else if (incomingByte >= 210 && incomingByte <= 211) {   // light
      lightCmd = incomingByte - 210;
    }    
    updateServo("S", (IS_REVERSE_STEERING ? map(steeringCmd, 0, 99, STEERING_MAX, STEERING_MIN) : map(steeringCmd, 0, 99, STEERING_MIN, STEERING_MAX)));
    updateServo("E", (IS_REVERSE_ENGINE ? map(engineCmd, 0, 99, 180, 0) : map(engineCmd, 0, 99, 0, 180)));
    updateLight(lightCmd);
    //Serial.print(steeringCmd); Serial.print(" | "); Serial.println(engineCmd); Serial.print(" | "); Serial.println(lightCmd);
  }
}


void updateServo(String type, int value) {
  if (type == "S") {
    steeringServo.write(value);
    steeringServo.refresh();
  } else if (type == "E") {
    engineServo.write(value);
    engineServo.refresh();
  }
}


void updateLight(byte lightValue) {
  if (lightValue == 1) {
    digitalWrite(LIGHT_PIN, HIGH);
  } else {
    digitalWrite(LIGHT_PIN, LOW);
  }
}


void imAlive() {
  for (byte i = 0; i < 3; i++) {
    digitalWrite(LIGHT_PIN, HIGH);
    delay(100);
    digitalWrite(LIGHT_PIN, LOW);
    delay(100);
  }
}
