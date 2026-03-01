
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
#include "MPU6050.h"

/*Hardware device member object*/
DeviceDriverSet_Motor AppMotor;
DeviceDriverSet_ULTRASONIC AppULTRASONIC;
MPU6050 mpu;

void ApplicationFunctionSet::ApplicationFunctionSet_Init(void)
{
  Serial.begin(9600);
  AppMotor.DeviceDriverSet_Motor_Init();
  AppULTRASONIC.DeviceDriverSet_ULTRASONIC_Init();
  

  #if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
    Wire.begin(); 
  #elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
    Fastwire::setup(400, true);
  #endif

  // Serial.println("Scanning...");

  // for (byte address = 1; address < 127; address++) {
  //   Wire.beginTransmission(address);
  //   if (Wire.endTransmission() == 0) {
  //     Serial.print("Found device at 0x");
  //     Serial.println(address, HEX);
  //   }
  // }

  /*Initialize device and check connection*/ 
  Serial.println("Initializing MPU...");
  mpu.initialize();
  Serial.println("Testing MPU6050 connection...");
  if(mpu.testConnection() ==  false){
    Serial.println("MPU6050 connection failed");
    // while(true);
  }
  else{
    Serial.println("MPU6050 connection successful");
  }

  int devStatus = mpu.dmpInitialize();

  Serial.print("dmp status: ");
  Serial.println(devStatus);

  Serial.println("Updating internal sensor offsets...\n");
  mpu.setXAccelOffset(0); //Set your accelerometer offset for axis X
  mpu.setYAccelOffset(0); //Set your accelerometer offset for axis Y
  mpu.setZAccelOffset(0); //Set your accelerometer offset for axis Z
  mpu.setXGyroOffset(0);  //Set your gyro offset for axis X
  mpu.setYGyroOffset(0);  //Set your gyro offset for axis Y
  mpu.setZGyroOffset(0);  //Set your gyro offset for axis Z
}

void ApplicationFunctionSet::ApplicationFunctionSet_SensorDataUpdate(void)
{
  AppULTRASONIC.DeviceDriverSet_ULTRASONIC_Get(&UltrasoundData_mm /*out*/); //超声波数据
  UltrasoundData_cm = UltrasoundData_mm / 10;
  int16_t ax, ay, az;
  int16_t gx, gy, gz;
  mpu.getRotation(&gx, &gy, &gz);
  Serial.print("a/g:\t");
  Serial.print(ax); Serial.print("\t");
  Serial.print(ay); Serial.print("\t");
  Serial.print(az); Serial.print("\t");
  Serial.print(gx); Serial.print("\t");
  Serial.print(gy); Serial.print("\t");
  Serial.println(gz);
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
