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

enum DriveMode
{
  Straight,
  Turning
};

int targetAngle = 0;

/* Turn power, greater means turn more CCW*/
int turnPower() {
  return fmod((targetAngle - Application_FunctionSet.MPUyaw) + 540, 360) - 180;
} 

int toneMillis = 0;
DriveMode driveMode;

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
  driveMode = DriveMode::Turning;
}

void loop()
{
  wdt_reset();
  Application_FunctionSet.ApplicationFunctionSet_SensorDataUpdate();

  switch (driveMode) {
    case DriveMode::Straight:
    toneMillis = millis();
    Application_FunctionSet.Drive(OwlBotMotionControl::Forward, 255);
    Serial.print("UltrasoundData: ");
    Serial.println(Application_FunctionSet.UltrasoundData_cm);
    if ((Application_FunctionSet.UltrasoundData_cm) < 20) {
      driveMode = DriveMode::Turning;
      if (targetAngle == 0) {
        targetAngle = 180;
      } else {
        targetAngle = 0;
      }
    }
    break;
    case DriveMode::Turning:
    if ((millis() - toneMillis) > 500) {
      toneMillis = millis();
      tone(5, 442, 250);
    }
    Serial.print("Turn power: ");
    Serial.println(turnPower());
    Application_FunctionSet.Drive(turnPower(), -1 * turnPower());
    if (turnPower() < 5 && turnPower() > -5) {
      driveMode = DriveMode::Straight;
      break;
    }
    break;
    default:
    break;
  }

  float dozerAdjust = 3 * max(-12, min(Application_FunctionSet.MPUpitch, 12));

  servo1.write(90 + dozerAdjust);
  servo2.write(90 - dozerAdjust);

  delay(50);
}
