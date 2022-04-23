//----------------------------------------------------------------------------------------------
// RX part for rc car with esc (ATtiny85 based and esc by motor driver chip)
// cmd package format:
//    first byte:  0-99 steering level
//    second byte: 100-199 throttle level
//    third byte: 210-211 switch (optional, not implemented)
//----------------------------------------------------------------------------------------------
#include <SoftwareSerial.h>
#include <SoftwareServo.h>

// SETTINGS
#define STEERING_MIN 30   // 0..90, 90 - center
#define STEERING_MAX 150  // 90..180, 90 - center
#define IS_REVERSE_STEERING false

#define RX_PIN PIN_PB3
#define TX_PIN PIN_PB4
#define STEERING_PIN PIN_PB2
#define ENGINE_PIN_FWD PIN_PB0
#define ENGINE_PIN_BWD PIN_PB1


SoftwareSerial swSerial(RX_PIN, TX_PIN);
SoftwareServo steeringServo;
byte steeringCmd = 50;
byte engineCmd = 50;
int failSafe = 0;


void setup() {
  swSerial.begin(9600);
  pinMode(ENGINE_PIN_FWD, OUTPUT);
  pinMode(ENGINE_PIN_BWD, OUTPUT);
  pinMode(TX_PIN, OUTPUT);
  steeringServo.attach(STEERING_PIN);
  steeringServo.setMinimumPulse(850);
  //steeringServo.setMaximumPulse(2000);
  imAlive();
}


void loop() {
  if (swSerial.available() > 0) {
    byte incomingByte = swSerial.read();
    //swSerial.println(incomingByte);
    if (incomingByte >= 0 && incomingByte <= 99) {           // steering
      steeringCmd = incomingByte;
    }
    else if (incomingByte >= 100 && incomingByte <= 199) {   // engine
      engineCmd = incomingByte - 100;
    }
    updateActuators(steeringCmd, engineCmd);
    failSafe = 0;
  } else {
    if (failSafe > 10000) {
      updateActuators(50, 50);
    }
    failSafe++;
  }
}


void updateActuators(byte steeringValue, byte engineValue) {
  steeringServo.write(IS_REVERSE_STEERING ? map(steeringValue, 0, 99, STEERING_MAX, STEERING_MIN) : map(steeringValue, 0, 99, STEERING_MIN, STEERING_MAX));
  delay(20);
  steeringServo.refresh();
  if (engineValue > 53) {
    analogWrite(ENGINE_PIN_FWD, map(engineValue, 52, 99, 10, 255));
    analogWrite(ENGINE_PIN_BWD, 0);
    digitalWrite(TX_PIN, LOW);
  } else  if (engineValue < 48) {
    analogWrite(ENGINE_PIN_FWD, 0);
    analogWrite(ENGINE_PIN_BWD, map(engineValue, 48, 0, 10, 255));
    digitalWrite(TX_PIN, LOW);
  } else {
    analogWrite(ENGINE_PIN_FWD, 0);
    analogWrite(ENGINE_PIN_BWD, 0);
    digitalWrite(TX_PIN, HIGH);
  }
}


void imAlive() {
  analogWrite(ENGINE_PIN_BWD, 0);
  analogWrite(ENGINE_PIN_FWD, 30); delay(100);
  analogWrite(ENGINE_PIN_FWD, 0);  delay(100);
  analogWrite(ENGINE_PIN_FWD, 30); delay(100);
  analogWrite(ENGINE_PIN_FWD, 0);  delay(100);
  analogWrite(ENGINE_PIN_FWD, 30); delay(200);
  analogWrite(ENGINE_PIN_FWD, 0);
}