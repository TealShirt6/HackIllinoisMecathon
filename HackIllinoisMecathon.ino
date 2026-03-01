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

ApplicationFunctionSet Application_FunctionSet;

Servo servo1;
Servo servo2;

void setup()
{
  servo1.attach(SERVO_1);
  servo2.attach(SERVO_2);
  // put your setup code here, to run once:
  Application_FunctionSet.ApplicationFunctionSet_Init();
  // Application_FunctionSet.ApplicationFunctionSet_Bootup();
  wdt_enable(WDTO_2S);
  servo1.write(90);
  servo2.write(90);
  Serial.begin(9600);
}

void loop()
{
  
  Application_FunctionSet.ApplicationFunctionSet_SensorDataUpdate();
  wdt_reset();
  Serial.println(Application_FunctionSet.UltrasoundData_cm);

  if ((Application_FunctionSet.UltrasoundData_cm) > 10) {
    // Application_FunctionSet.Drive(OwlBotMotionControl::Forward, 255);
    servo1.write(75);
    servo2.write(105);
  } else {
    // Application_FunctionSet.Drive(OwlBotMotionControl::Backward, 255);
    servo1.write(105);
    servo2.write(75);
  }
  delay(250);

  // Application_FunctionSet.ApplicationFunctionSet_KeyCommand();
  // Application_FunctionSet.ApplicationFunctionSet_RGB();
  // Application_FunctionSet.ApplicationFunctionSet_Rocker();
  // Application_FunctionSet.ApplicationFunctionSet_Tracking();
  // Application_FunctionSet.ApplicationFunctionSet_Exploration();
  // Application_FunctionSet.ApplicationFunctionSet_Obstacle();
  // Application_FunctionSet.ApplicationFunctionSet_Standby();
  // Application_FunctionSet.ApplicationFunctionSet_Expression();
  // Application_FunctionSet.ApplicationFunctionSet_SerialPortDataAnalysis();

  // Application_FunctionSet.CMD_MotorControl_xxx0();
  // Application_FunctionSet.CMD_CarControlTimeLimit_xxx0();
  // Application_FunctionSet.CMD_CarControlNoTimeLimit_xxx0();
  // Application_FunctionSet.CMD_MotorControlSpeed_xxx0();
  // Application_FunctionSet.CMD_VoiceControl_xxx0();
  // Application_FunctionSet.CMD_LightingControlTimeLimit_xxx0();
  // Application_FunctionSet.CMD_LightingControlNoTimeLimit_xxx0();
  // Application_FunctionSet.CMD_LEDCustomExpressionControl_xxx0();
  // Application_FunctionSet.CMD_LEDNumberDisplayControl_xxx0();
  // Application_FunctionSet.CMD_TrajectoryControl_xxx0();
  // Application_FunctionSet.CMD_ClearAllFunctions_xxx0();
}
