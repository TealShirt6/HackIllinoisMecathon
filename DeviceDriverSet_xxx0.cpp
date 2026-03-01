/*
 * @Description: In User Settings Edit
 * @Author: HOU Changhua 
 * @Date: 2019-07-11 13:40:52
 * @LastEditTime: 2019-10-15 11:51:54
 * @LastEditors: Please set LastEditors
 */

#include "DeviceDriverSet_xxx0.h"
#include "I2Cdev.h"
#include "MPU6050_6Axis_MotionApps20.h"

MPU6050 mpu;

/*ULTRASONIC*/
void DeviceDriverSet_ULTRASONIC::DeviceDriverSet_ULTRASONIC_Init(void)
{
  Wire.begin();
}

void DeviceDriverSet_ULTRASONIC::DeviceDriverSet_ULTRASONIC_Get(uint16_t *ULTRASONIC_Get /*out*/)
{
  unsigned dat[2] = {0};
  Wire.requestFrom(0x07, 1); 
  if (Wire.available() > 0)
  {
    dat[0] = Wire.read();
  }
  Wire.requestFrom(0x07, 1); 
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
  Wire.requestFrom(0x07, 1);
  if (Wire.available() > 0)
  {
    dat[0] = Wire.read();
  }

  Wire.requestFrom(0x07, 1); 
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
void DeviceDriverSet_Motor::DeviceDriverSet_Motor_control(boolean direction_A, uint8_t speed_A, 
                                                          boolean direction_B, uint8_t speed_B, 
                                                          boolean controlED                     
                                                          )                                    
{
   if (controlED == control_enable)
  {
    digitalWrite(PIN_Motor_STBY, HIGH); 
    {                                   
      switch (direction_A)             
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

int const INTERRUPT_PIN = 2;
/*---MPU6050 Control/Status Variables---*/
bool DMPReady = false;  // Set true if DMP init was successful
uint8_t MPUIntStatus;   // Holds actual interrupt status byte from MPU
uint8_t devStatus;      // Return status after each device operation (0 = success, !0 = error)
uint16_t packetSize;    // Expected DMP packet size (default is 42 bytes)
uint8_t FIFOBuffer[64]; // FIFO storage buffer

/*---Orientation/Motion Variables---*/ 
Quaternion quaternion;           // [w, x, y, z]         Quaternion container
VectorInt16 aa;         // [x, y, z]            Accel sensor measurements
VectorInt16 gy;         // [x, y, z]            Gyro sensor measurements
VectorInt16 aaReal;     // [x, y, z]            Gravity-free accel sensor measurements
VectorInt16 aaWorld;    // [x, y, z]            World-frame accel sensor measurements
VectorFloat gravity;    // [x, y, z]            Gravity vector
float euler[3];         // [psi, theta, phi]    Euler angle container
float ypr[3];           // [yaw, pitch, roll]   Yaw/Pitch/Roll container and gravity vector

/*-Packet structure for InvenSense teapot demo-*/ 
uint8_t teapotPacket[14] = { '$', 0x02, 0, 0, 0, 0, 0, 0, 0, 0, 0x00, 0x00, '\r', '\n' };

/*------Interrupt detection routine------*/
volatile bool MPUInterrupt = false;     // Indicates whether MPU6050 interrupt pin has gone high
void DMPDataReady() {
  MPUInterrupt = true;
}

/*STM8S003F3_MPU6050*/
void DeviceDriverSet_MPU6050::DeviceDriverSet_MPU6050_Init(void)
{
  #if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
    Wire.begin();
    Wire.setClock(400000); // 400kHz I2C clock. Comment on this line if having compilation difficulties
  #elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
    Fastwire::setup(400, true);
  #endif
  

  Serial.begin(115200); //115200 is required for Teapot Demo output
  while (!Serial);

  /*Initialize device*/
  Serial.println(F("Initializing I2C devices..."));
  mpu.initialize();
  pinMode(INTERRUPT_PIN, INPUT);

  /*Verify connection*/
  Serial.println(F("Testing MPU6050 connection..."));
  if(mpu.testConnection() == false){
    Serial.println("MPU6050 connection failed");
    // while(true);
  }
  else {
    Serial.println("MPU6050 connection successful");
  }

  /* Have user send character over serial to initialize dmp */
  /*Wait for Serial input*/
  // Serial.println(F("\nSend any character to begin: "));
  // while (Serial.available() && Serial.read()); // Empty buffer
  // while (!Serial.available());                 // Wait for data
  // while (Serial.available() && Serial.read()); // Empty buffer again

  /* Initializate and configure the DMP*/
  Serial.println(F("Initializing DMP..."));
  devStatus = mpu.dmpInitialize();

  /* Supply your gyro offsets here, scaled for min sensitivity */
  mpu.setXGyroOffset(0);
  mpu.setYGyroOffset(0);
  mpu.setZGyroOffset(0);
  mpu.setXAccelOffset(0);
  mpu.setYAccelOffset(0);
  mpu.setZAccelOffset(0);

  /* Making sure it worked (returns 0 if so) */ 
  if (devStatus == 0) {
    mpu.CalibrateAccel(6);  // Calibration Time: generate offsets and calibrate our MPU6050
    mpu.CalibrateGyro(6);
    Serial.println("These are the Active offsets: ");
    mpu.PrintActiveOffsets();
    Serial.println(F("Enabling DMP..."));   //Turning ON DMP
    mpu.setDMPEnabled(true);

    /*Enable Arduino interrupt detection*/
    Serial.print(F("Enabling interrupt detection (Arduino external interrupt "));
    Serial.print(digitalPinToInterrupt(INTERRUPT_PIN));
    Serial.println(F(")..."));
    attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN), DMPDataReady, RISING);
    MPUIntStatus = mpu.getIntStatus();

    /* Set the DMP Ready flag so the main loop() function knows it is okay to use it */
    Serial.println(F("DMP ready! Waiting for first interrupt..."));
    DMPReady = true;
    packetSize = mpu.dmpGetFIFOPacketSize(); //Get expected DMP packet size for later comparison
  } 
  else {
    Serial.print(F("DMP Initialization failed (code ")); //Print the error code
    Serial.print(devStatus);
    Serial.println(F(")"));
    // 1 = initial memory load failed
    // 2 = DMP configuration updates failed
  }
  pinMode(LED_BUILTIN, OUTPUT);

}

void DeviceDriverSet_MPU6050::DeviceDriverSet_MPU6050_getYawPitchRoll(float *yaw, float *pitch, float *roll)
{
  if (!DMPReady) return; // Stop the program if DMP programming fails.
    
  /* Read a packet from FIFO */
  if (mpu.dmpGetCurrentFIFOPacket(FIFOBuffer)) { // Get the Latest packet 
    /* Display Euler angles in degrees */
    mpu.dmpGetQuaternion(&quaternion, FIFOBuffer);
    mpu.dmpGetGravity(&gravity, &quaternion);
    mpu.dmpGetYawPitchRoll(ypr, &quaternion, &gravity);
    *yaw = ypr[0] * 180/M_PI;
    *roll = ypr[1] * 180/M_PI;
    *pitch = ypr[2] * 180/M_PI;
    // Serial.print("ypr\t");
    // Serial.print(ypr[0] * 180/M_PI);
    // Serial.print("\t");
    // Serial.print(ypr[1] * 180/M_PI);
    // Serial.print("\t");
    // Serial.println(ypr[2] * 180/M_PI);
  }
}
