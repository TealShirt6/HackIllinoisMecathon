/*
 * @Description: In User Settings Edit
 * @Author: HOU Changhua 
 * @Date: 2019-07-11 13:40:52
 * @LastEditTime: 2019-10-15 11:51:54
 * @LastEditors: Please set LastEditors
 */

#include "DeviceDriverSet_xxx0.h"

/*ULTRASONIC*/
void DeviceDriverSet_ULTRASONIC::DeviceDriverSet_ULTRASONIC_Init(void)
{
  Wire.begin();
}

void DeviceDriverSet_ULTRASONIC::DeviceDriverSet_ULTRASONIC_Get(uint16_t *ULTRASONIC_Get /*out*/)
{
  unsigned dat[2] = {0};
  Wire.requestFrom(0x07, 1); //从器件读取一位数
  if (Wire.available() > 0)
  {
    dat[0] = Wire.read();
  }
  Wire.requestFrom(0x07, 1); //从器件读取一位数
  if (Wire.available() > 0)
  {
    dat[1] = Wire.read();
  }

  *ULTRASONIC_Get = ((dat[0] << 8) | dat[1]);
}

#if _Test_DeviceDriverSet
void DeviceDriverSet_ULTRASONIC::DeviceDriverSet_ULTRASONIC_Test(void)
{
  unsigned dat[2] = {0};
  Wire.requestFrom(0x07, 1); //从器件读取一位数
  if (Wire.available() > 0)
  {
    dat[0] = Wire.read();
  }

  Wire.requestFrom(0x07, 1); //从器件读取一位数
  if (Wire.available() > 0)
  {
    dat[1] = Wire.read();
  }
  _delay(100);
  Serial.print("ULTRASONIC=");
  Serial.println((dat[0] << 8) | dat[1]);
}
#endif

extern unsigned long _millis()
{
  return millis() * TimeCompensation;
}
extern void _delay(unsigned long ms)
{
  delay(ms / TimeCompensation);
}

/*Motor control*/
void DeviceDriverSet_Motor::DeviceDriverSet_Motor_Init(void)
{
   pinMode(PIN_Motor_PWMA, OUTPUT);
  pinMode(PIN_Motor_PWMB, OUTPUT);
  pinMode(PIN_Motor_AIN, OUTPUT);
  pinMode(PIN_Motor_BIN, OUTPUT);
  pinMode(PIN_Motor_STBY, OUTPUT);
}

#if _Test_DeviceDriverSet
void DeviceDriverSet_Motor::DeviceDriverSet_Motor_Test(void)
{
  //digitalWrite(PIN_Motor_STBY, HIGH);
  digitalWrite(PIN_Motor_AIN, LOW);
  digitalWrite(PIN_Motor_BIN, HIGH);
 analogWrite(PIN_Motor_PWMA, 150);
  analogWrite(PIN_Motor_PWMB, 150);
}
#endif

/*
 Motor_control：AB / 方向、速度
*/
void DeviceDriverSet_Motor::DeviceDriverSet_Motor_control(boolean direction_A, uint8_t speed_A, //A组电机参数
                                                          boolean direction_B, uint8_t speed_B, //B组电机参数
                                                          boolean controlED                     //AB使能允许 true
                                                          )                                     //电机控制
{
   if (controlED == control_enable) //使能允许？
  {
    digitalWrite(PIN_Motor_STBY, HIGH); //开启
    {                                   //A...
      switch (direction_A)              //方向控制
      {
      case direction_just:
        digitalWrite(PIN_Motor_AIN, HIGH);
        analogWrite(PIN_Motor_PWMA, speed_A);
        break;
      case direction_back:
        digitalWrite(PIN_Motor_AIN, LOW);
        analogWrite(PIN_Motor_PWMA, speed_A);
        break;
      case direction_void:
        analogWrite(PIN_Motor_PWMA, 0);
        break;
      default:
        analogWrite(PIN_Motor_PWMA, 0);
        break;
      }
    }
    { //B...
      switch (direction_B)
      {
      case direction_just:
        digitalWrite(PIN_Motor_BIN, HIGH);
        analogWrite(PIN_Motor_PWMB, speed_B);
        break;
      case direction_back:
        digitalWrite(PIN_Motor_BIN, LOW);
        analogWrite(PIN_Motor_PWMB, speed_B);

        break;
      case direction_void:
        analogWrite(PIN_Motor_PWMB, 0);
        break;
      default:
        analogWrite(PIN_Motor_PWMB, 0);
        break;
      }
    }
  }
  else
  {
    digitalWrite(PIN_Motor_STBY, LOW); //关闭
    analogWrite(PIN_Motor_PWMA, 0);
    analogWrite(PIN_Motor_PWMB, 0);
    return;
  }
}

/*STM8S003F3_MPU6050*/
void DeviceDriverSet_MPU6050::DeviceDriverSet_MPU6050_Init(void)
{
  Wire.begin();
  Wire.beginTransmission(STM8S003F3_MPU6050_devAddr);
  Wire.write(110);
  Wire.endTransmission();
  delay(50);
}

void DeviceDriverSet_MPU6050::DeviceDriverSet_MPU6050_dveGetEulerAngles(float *is_yaw)
{
  // uint8_t STM8S003F3_MPU6050_IIC_buff[6];
  // uint8_t a = 0;
  // Wire.requestFrom(STM8S003F3_MPU6050_devAddr, 6); // request 6 bytes from slave device #2
  // while (Wire.available())                         // slave may send less than requested
  // {
  //   STM8S003F3_MPU6050_IIC_buff[a++] = Wire.read(); // receive a byte as character
  // }

  // if ((STM8S003F3_MPU6050_IIC_buff[0] == 0XA1) && (STM8S003F3_MPU6050_IIC_buff[5] == 0XB1))
  // {
  //   //*gyro = ((STM8S003F3_MPU6050_IIC_buff[1] << 8) | (STM8S003F3_MPU6050_IIC_buff[2])) / 1000.00;
  //   *is_yaw = ((STM8S003F3_MPU6050_IIC_buff[3] << 8) | (STM8S003F3_MPU6050_IIC_buff[4])) / 100.00;
  // }
  // else
  // {
  //   /* code */
  //   //Serial.println("Contact Changhua :STM8S003F3_MPU6050 data error"); // print the character
  //   return;
  // }
  float gyro, yaw;
  DeviceDriverSet_MPU6050::DeviceDriverSet_MPU6050_dveGetEulerAngles(&gyro, &yaw);
  *is_yaw = yaw;
}

void DeviceDriverSet_MPU6050::DeviceDriverSet_MPU6050_dveGetEulerAngles(float *gyro, float *is_yaw)
{
  uint8_t STM8S003F3_MPU6050_IIC_buff[6];
  uint8_t a = 0;
  Wire.requestFrom(STM8S003F3_MPU6050_devAddr, 6); // request 6 bytes from slave device #2
  while (Wire.available())                         // slave may send less than requested
  {
    STM8S003F3_MPU6050_IIC_buff[a++] = Wire.read(); // receive a byte as character
  }

  if ((STM8S003F3_MPU6050_IIC_buff[0] == 0XA1) && (STM8S003F3_MPU6050_IIC_buff[5] == 0XB1))
  {
    *gyro = ((STM8S003F3_MPU6050_IIC_buff[1] << 8) | (STM8S003F3_MPU6050_IIC_buff[2])) / 1000.00;
    *is_yaw = ((STM8S003F3_MPU6050_IIC_buff[3] << 8) | (STM8S003F3_MPU6050_IIC_buff[4])) / 100.00;
  }
  else
  {
    /* code */
    //Serial.println("Contact Changhua :STM8S003F3_MPU6050 data error"); // print the character
    return;
  }
}
#if _Test_DeviceDriverSet
void DeviceDriverSet_MPU6050::DeviceDriverSet_MPU6050_Test(void)
{

  uint8_t STM8S003F3_MPU6050_IIC_buff[4];
  uint8_t a = 0;
  Wire.requestFrom(STM8S003F3_MPU6050_devAddr, 4); // request 6 bytes from slave device #2
  while (Wire.available())                         // slave may send less than requested
  {
    STM8S003F3_MPU6050_IIC_buff[a++] = Wire.read(); // receive a byte as character
  }

  if ((STM8S003F3_MPU6050_IIC_buff[0] == 0XA1) && (STM8S003F3_MPU6050_IIC_buff[3] == 0XB1))
  {
    Serial.print("STM8S003F3_MPU6050->is_yaw:      ");
    Serial.println((STM8S003F3_MPU6050_IIC_buff[1] << 8) | (STM8S003F3_MPU6050_IIC_buff[2]));
    Serial.println("...................................."); // print the character
  }
  else
  {
    /* code */
    Serial.println("STM8S003F3_MPU6050 data error"); // print the character
  }
}
#endif
