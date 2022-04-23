//----------------------------------------------------------------------------------------------
// TX part for rc (ATtiny13/JDY-40 based)
// MicroCore by MCUdude (https://github.com/MCUdude/MicroCore)
// !Don't forget about section "Internal oscillator calibration"!
// cmd package format:
//    first byte:  0-99 steering
//    second byte: 100-199 engine
//    third byte:  210-211 switch
//----------------------------------------------------------------------------------------------

#define SWITCH_PIN    PB2      //#7
#define STEERING_PIN  A3       //#2
#define ENGINE_PIN    A2       //#3


#include <EEPROM.h>
#include "button.h"

button swtch(SWITCH_PIN);
bool switchState = false;


void setup() {
  uint8_t cal = EEPROM.read(0);
  if (cal < 0x80) OSCCAL = cal;
  Serial.begin();                //defined 9600 in core_settings.h
  pinMode(STEERING_PIN, INPUT);
  pinMode(ENGINE_PIN, INPUT);
}


void loop() {
  if (swtch.click()) {
    switchState = !switchState;
  }
  static uint32_t tmr = 0;
  if (millis() - tmr > 60) {
    tmr = millis();
    byte buf[3];
    buf[0] = map(analogRead(STEERING_PIN), 0, 1023, 0, 99);
    buf[1] = map(analogRead(ENGINE_PIN), 0, 1023, 100, 199);
    buf[2] = switchState ? 211 : 210;
    Serial.write(buf, sizeof(buf));
    //Serial.print(buf[0]); Serial.print(" - "); Serial.print(buf[1]); Serial.print(" - "); Serial.println(buf[2]);
  }
}
