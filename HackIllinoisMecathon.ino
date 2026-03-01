/*
 * @Author: ELEGOO
 * @Date: 2019-07-10 16:46:17
 * @LastEditTime: 2021-01-06 16:00:26
 * @LastEditors: Changhua
 * @Description: OwlBot Tank Kit
 * @FilePath: 
 */

#include <avr/wdt.h>
#include "ApplicationFunctionSet_xxx0.h"
#include <hardwareSerial.h>
#include <Servo.h>

#define SERVO_1 11
#define SERVO_2 3

#define PRINT

ApplicationFunctionSet Application_FunctionSet;

Servo servo1;
Servo servo2;

void setup()
{
  servo1.attach(SERVO_1);
  servo2.attach(SERVO_2);
  // put your setup code here, to run once:
  Application_FunctionSet.ApplicationFunctionSet_Init();
  wdt_enable(WDTO_2S);
  servo1.write(90);
  servo2.write(90);
  Serial.begin(115200);
}

void loop()
{
  wdt_reset();
  Application_FunctionSet.ApplicationFunctionSet_SensorDataUpdate();
  // Serial.println(Application_FunctionSet.UltrasoundData_cm);



  // if ((Application_FunctionSet.UltrasoundData_cm) > 20) {
  //   Application_FunctionSet.Drive(OwlBotMotionControl::Forward, 255);
  //   servo1.write(75);
  //   servo2.write(105);
  // } else {
  //   Application_FunctionSet.Drive(OwlBotMotionControl::Backward, 255);
  //   servo1.write(105);
  //   servo2.write(75);
  // }

  float dozerAdjust = 3 * max(-12, min(Application_FunctionSet.MPUpitch, 12));
  Serial.println(dozerAdjust);

  servo1.write(90 + dozerAdjust);
  servo2.write(90 - dozerAdjust);
  // if (Application_FunctionSet.MPUpitch < -5) {
  //   servo1.write(75);
  //   servo2.write(105);
  // } else if (Application_FunctionSet.MPUpitch > 5) {
  //   servo1.write(105);
  //   servo2.write(75);
  // } else {
  //   servo1.write(90);
  //   servo2.write(90);
  // }

  delay(100);
}
