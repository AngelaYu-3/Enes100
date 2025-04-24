#ifndef DFRTank_H_
#define DFRTank_H_


#include <Arduino.h>


//// PIN ASSIGNMENTS ////
// Motors
#define PIN_MOTOR_1_FORWARD 11
#define PIN_MOTOR_1_REVERSE 10
#define PIN_MOTOR_2_FORWARD 9
#define PIN_MOTOR_2_REVERSE 8
#define PIN_MOTOR_3_FORWARD 4
#define PIN_MOTOR_3_REVERSE 5
#define PIN_MOTOR_4_FORWARD 2
#define PIN_MOTOR_4_REVERSE 3
// Bump Sensors
#define PIN_BUMP_FRONT 32
#define PIN_BUMP_REAR 33
// Ultrasonic Sensors
#define PIN_US_FRONT_TRIG 18
#define PIN_US_FRONT_ECHO 19
#define PIN_US_REAR_TRIG 27
#define PIN_US_REAR_ECHO 26
// Infrared Sensors
#define PIN_IR_1 15
#define PIN_IR_2 17
#define PIN_IR_3 14
#define PIN_IR_4 16
// WiFi Modules
#define PIN_WIFI_TX 52
#define PIN_WIFI_RX 50


class TankClient {

public:

  TankClient();
  void begin();  // required on startup for setting pinModes

  // Functions available on both simulator and physical tanks
  void turnOffMotors();  // sets all motor speeds to 0
  void setLeftMotorPWM(int pwm);   // sets motors 1 and 2 speeds to specified pwm (pwm: -255 to 255)
  void setRightMotorPWM(int pwm);  // sets motors 3 and 4 speeds to specified pwm (pwm: -255 to 255)
  float readDistanceSensor(int sensorId);  // reads and returns distance in meters (maximum of 1) from specified ultrasonic distance sensor (only valid for sensorIds 1 or 7)

  // Functions only available on physical tanks
  void setMotorPWM(int motorId, int pwm);  // sets the speed of the specifed motor to the specified pwm (only valid for motorIds 1 through 4) (pwm: -255 to 255)
  int readIrSensor(int sensorId);  // reads and returns the state of the specified infrared sensor (only valid for sensorIds 1 through 4)
  int readBumpSensor(int sensorId);  // reads and returns the state of the specified infrared sensor (only valid for sensorIds 1 or 7)
};

#endif
