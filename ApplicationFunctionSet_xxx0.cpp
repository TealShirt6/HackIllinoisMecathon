
/*
   @Description: Application Function Set
   @Author: HOU Changhua
   @Date: 2019-07-12 18:30:51
   @LastEditTime: 2019-10-14 18:10:30
   @LastEditors: Please set LastEditors
*/

#define speed_Max 255

#include "ApplicationFunctionSet_xxx0.h"
#include "DeviceDriverSet_xxx0.h"

/*Hardware device member object*/
DeviceDriverSet_Motor AppMotor;
DeviceDriverSet_ULTRASONIC AppULTRASONIC;
DeviceDriverSet_MPU6050 AppMPU;

void ApplicationFunctionSet::ApplicationFunctionSet_Init(void)
{
  AppMotor.DeviceDriverSet_Motor_Init();
  AppULTRASONIC.DeviceDriverSet_ULTRASONIC_Init();
  AppMPU.DeviceDriverSet_MPU6050_Init();
}

void ApplicationFunctionSet::ApplicationFunctionSet_SensorDataUpdate(void)
{
  AppULTRASONIC.DeviceDriverSet_ULTRASONIC_Get(&UltrasoundData_mm /*out*/);
  UltrasoundData_cm = UltrasoundData_mm / 10;
  AppMPU.DeviceDriverSet_MPU6050_getYawPitchRoll(&MPUyaw, &MPUpitch, &MPUroll /* out */);
}

/*
  --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
  @ Function prototype:   static void ApplicationFunctionSet_OwlBotMotionControl(OwlBotMotionControl direction, uint8_t speed)
  @ Functional function:  Owl Car motion state control
  @ Input parameters:     1# direction:Forward（1）、Backward（2）、 LeftForward（3）、RightForward（4）、LeftBackward（5）、RightBackward（6）、stop_it（6）
                        2# speed(0--255)
  @ Output parameters:    none
  @ Other Notes:          This is the static method of the applicationfunctionset layer < call the hardware driver layer devicedriverset > Motor > control interface implementation >
  $ Shenzhen, China:Elegoo & HOU Changhua & 2019-09
  --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
*/
static void ApplicationFunctionSet_OwlBotMotionControl(OwlBotMotionControl direction, uint8_t speed)
{
  ApplicationFunctionSet Application_FunctionSet;
  switch (direction)
  {
    case /* constant-expression */ Forward:
      /* code */
      AppMotor.DeviceDriverSet_Motor_control(/*direction_A*/ direction_just, /*speed_A*/ speed,
          /*direction_B*/ direction_just, /*speed_B*/ speed, /*controlED*/ control_enable); //Motor control
      break;
    case /* constant-expression */ Left:
      /* code */
      AppMotor.DeviceDriverSet_Motor_control(/*direction_A*/ direction_just, /*speed_A*/ speed,
          /*direction_B*/ direction_back, /*speed_B*/ speed, /*controlED*/ control_enable); //Motor control
      break;
    case /* constant-expression */ Right:
      /* code */
      AppMotor.DeviceDriverSet_Motor_control(/*direction_A*/ direction_back, /*speed_A*/ speed,
          /*direction_B*/ direction_just, /*speed_B*/ speed, /*controlED*/ control_enable); //Motor control
      break;
    case /* constant-expression */ Backward:
      /* code */
      AppMotor.DeviceDriverSet_Motor_control(/*direction_A*/ direction_back, /*speed_A*/ speed,
          /*direction_B*/ direction_back, /*speed_B*/ speed, /*controlED*/ control_enable); //Motor control
      break;
    case /* constant-expression */ LeftForward:
      /* code */
      AppMotor.DeviceDriverSet_Motor_control(/*direction_A*/ direction_just, /*speed_A*/ speed - 130,
          /*direction_B*/ direction_just, /*speed_B*/ speed, /*controlED*/ control_enable); //Motor control
      break;
    case /* constant-expression */ RightForward:
      /* code */
      AppMotor.DeviceDriverSet_Motor_control(/*direction_A*/ direction_just, /*speed_A*/ speed - 130,
          /*direction_B*/ direction_just, /*speed_B*/ speed, /*controlED*/ control_enable); //Motor control
      break;
    case /* constant-expression */ LeftBackward:
      /* code */
      AppMotor.DeviceDriverSet_Motor_control(/*direction_A*/ direction_back, /*speed_A*/ speed,
          /*direction_B*/ direction_back, /*speed_B*/ speed - 130, /*controlED*/ control_enable); //Motor control
      break;
    case /* constant-expression */ RightBackward:
      /* code */
      AppMotor.DeviceDriverSet_Motor_control(/*direction_A*/ direction_back, /*speed_A*/ speed - 130,
          /*direction_B*/ direction_back, /*speed_B*/ speed, /*controlED*/ control_enable); //Motor control
      break;
    case /* constant-expression */ stop_it:
      /* code */
      AppMotor.DeviceDriverSet_Motor_control(/*direction_A*/ direction_void, /*speed_A*/ 0,
          /*direction_B*/ direction_void, /*speed_B*/ 0, /*controlED*/ control_enable); //Motor control
      break;
    default:
      break;
  }
}

#define Max_speed 255
void ApplicationFunctionSet::Test(void)
{
  for (int i = 0; i <= stop_it; i++)
  {
    ApplicationFunctionSet_OwlBotMotionControl(i, Max_speed);
    delay(1000);
  }
}

void ApplicationFunctionSet::Drive(OwlBotMotionControl direction, uint8_t speed) {
    ApplicationFunctionSet_OwlBotMotionControl(direction, speed);
}

void ApplicationFunctionSet::Drive(float leftSpeed, float rightSpeed) {
  int p = 15;

  boolean dirA = leftSpeed > 0 ? direction_just : direction_back;
  float spdA = leftSpeed > 0 ? leftSpeed : -leftSpeed;
  if (spdA * p < 128)
    spdA = 0;
  else
    spdA = min(spdA * p, 255);
  boolean dirB = rightSpeed > 0 ? direction_just : direction_back;
  float spdB = rightSpeed > 0 ? rightSpeed : -rightSpeed;
  if (spdB * p < 128)
    spdB = 0;
  else
    spdB = min(spdB * p, 255);

  Serial.print("dirA: ");
  Serial.print(dirA);
  Serial.print(",dirB: ");
  Serial.print(dirB);
  Serial.print(",spdA: ");
  Serial.print(spdA);
  Serial.print(",spdB: ");
  Serial.println(spdB);

      // ApplicationFunctionSet_OwlBotMotionControl(OwlBotMotionControl::Forward, 255);

  AppMotor.DeviceDriverSet_Motor_control(dirA, (uint8_t) spdA, dirB, (uint8_t) spdB, control_enable);
  // AppMotor.DeviceDriverSet_Motor_control(leftSpeed > 0 ? direction_just : direction_back, leftSpeed > 0 ? leftSpeed : -leftSpeed,
                                        // rightSpeed > 0 ? direction_just : direction_back, rightSpeed > 0 ? rightSpeed : -rightSpeed, control_enable); 
}
