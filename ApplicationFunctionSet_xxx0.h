/*
 * @Description: In User Settings Edit
 * @Author:  HOU Changhua
 * @Date: 2019-07-12 18:30:51
 * @LastEditTime: 2019-10-14 17:50:22
 * @LastEditors: Please set LastEditors
 */

#ifndef _ApplicationFunctionSet_xxx0_H_
#define _ApplicationFunctionSet_xxx0_H_

#include <arduino.h>

/*Motion direction control sequence*/
enum OwlBotMotionControl
{
  Forward,       //(1)
  Backward,      //(2)
  Left,          //(3)
  Right,         //(4)
  LeftForward,   //(5)
  LeftBackward,  //(6)
  RightForward,  //(7)
  RightBackward, //(8)
  stop_it        //(9)
};            //direction

class ApplicationFunctionSet
{
public:
  uint16_t UltrasoundData_cm;
  uint16_t UltrasoundData_mm;
  float MPUyaw;
  float MPUpitch;
  float MPUroll;
  void ApplicationFunctionSet_Init(void);
  void Test(void);
  void Drive(OwlBotMotionControl direction, uint8_t speed);
  void ApplicationFunctionSet::ApplicationFunctionSet_SensorDataUpdate(void);
};

#endif
