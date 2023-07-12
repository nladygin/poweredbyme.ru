/*
   -- rx-tx-5ch --
   
   This source code of graphical user interface 
   has been generated automatically by RemoteXY editor.
   To compile this code using RemoteXY library 3.1.8 or later version 
   download by link http://remotexy.com/en/library/
   To connect using RemoteXY mobile app by link http://remotexy.com/en/download/                   
     - for ANDROID 4.11.1 or later version;
     - for iOS 1.9.1 or later version;
    
   This source code is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.    
*/

//////////////////////////////////////////////
//        RemoteXY include library          //
//////////////////////////////////////////////

// RemoteXY select connection mode and include library 
#define REMOTEXY_MODE__ESP8266WIFI_LIB_POINT
#include <ESP8266WiFi.h>

#include <RemoteXY.h>
#include <Servo.h>

// RemoteXY connection settings 
#define REMOTEXY_WIFI_SSID "poweredbyme"
#define REMOTEXY_WIFI_PASSWORD "1234567890"
#define REMOTEXY_SERVER_PORT 6377


// RemoteXY configurate  
#pragma pack(push, 1)
uint8_t RemoteXY_CONF[] =   // 52 bytes
  { 255,5,0,0,0,45,0,16,31,1,5,32,7,48,49,49,2,26,31,4,
  0,7,5,10,37,2,26,4,0,46,4,10,37,2,26,2,1,21,19,20,
  9,2,26,31,31,79,78,0,79,70,70,0 };
  
// this structure defines all the variables and events of your control interface 
struct {

    // input variables
  int8_t joystick_x; // from -100 to 100  
  int8_t joystick_y; // from -100 to 100  
  int8_t slider_1; // =0..100 slider position 
  int8_t slider_2; // =0..100 slider position 
  uint8_t switch_on_off; // =1 if switch ON and =0 if OFF 

    // other variable
  uint8_t connect_flag;  // =1 if wire connected, else =0 

} RemoteXY;
#pragma pack(pop)

/////////////////////////////////////////////
//           END RemoteXY include          //
/////////////////////////////////////////////

// SETTINGS
#define STEERING_RANGE 5         // 10 - max, 1 - min
#define IS_STEERING_REVERS false
#define SLIDER1_RANGE 10         // 10 - max, 1 - min
#define IS_SLIDER1_REVERS false
#define SLIDER2_RANGE 10         // 10 - max, 1 - min
#define IS_SLIDER2_REVERS false


#define PIN_SWITCH 2
#define ENGINE_PIN_FWD 13
#define ENGINE_PIN_BWD 15
#define SERVO_STEERING_PIN 12
#define SERVO_SLIDER1_PIN 14
#define SERVO_SLIDER2_PIN 16

int SERVO_STEERING_CENTER = 1550;
int SERVO_SLIDER1_START = 500;
int SERVO_SLIDER2_START = 500;
Servo servo_steering;
Servo servo_slider1;
Servo servo_slider2;

void setup() {
  RemoteXY_Init (); 
  pinMode(ENGINE_PIN_FWD, OUTPUT);
  pinMode(ENGINE_PIN_BWD, OUTPUT);
  pinMode(PIN_SWITCH, OUTPUT);
  servo_steering.attach(SERVO_STEERING_PIN);
  servo_slider1.attach(SERVO_SLIDER1_PIN);
  servo_slider2.attach(SERVO_SLIDER2_PIN);
}

void loop() { 
  RemoteXY_Handler ();
  if (RemoteXY.connect_flag == 1) {
    updateSteering(RemoteXY.joystick_x);
    updateEngine(RemoteXY.joystick_y);
    updateSlider1(RemoteXY.slider_1);
    updateSlider2(RemoteXY.slider_2);
    updateSwitch(RemoteXY.switch_on_off);
  } else {
    updateSteering(0);
    updateEngine(0);
    updateSlider1(0);
    updateSlider2(0);
    updateSwitch(0);
  }
}

void updateSteering(int8_t steering_value) {
  steering_value = (IS_STEERING_REVERS) ? -1*steering_value : steering_value;
  servo_steering.writeMicroseconds(SERVO_STEERING_CENTER + steering_value*STEERING_RANGE);
}

void updateEngine(int8_t engine_value) {
  int v = (int) round(2.55*sqrt(100*abs(engine_value)));
  if (engine_value > 1) {
    analogWrite(ENGINE_PIN_FWD, v);
    analogWrite(ENGINE_PIN_BWD, 0);
  } else  if (engine_value < -1) {
    analogWrite(ENGINE_PIN_FWD, 0);
    analogWrite(ENGINE_PIN_BWD, v);
  } else {
    analogWrite(ENGINE_PIN_FWD, 0);
    analogWrite(ENGINE_PIN_BWD, 0);
  }
}

void updateSlider1(int8_t slider_value) {
  slider_value = (IS_SLIDER1_REVERS) ? -1*slider_value : slider_value;
  servo_slider1.writeMicroseconds(SERVO_SLIDER1_START + 2*slider_value*SLIDER1_RANGE);
}

void updateSlider2(int8_t slider_value) {
  slider_value = (IS_SLIDER2_REVERS) ? -1*slider_value : slider_value;
  servo_slider2.writeMicroseconds(SERVO_SLIDER2_START + 2*slider_value*SLIDER2_RANGE);
}

void updateSwitch(uint8_t switch_value) {
    digitalWrite(PIN_SWITCH, (switch_value==0)?LOW:HIGH);
}