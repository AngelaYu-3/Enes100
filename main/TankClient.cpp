#include "TankClient.h"


TankClient::TankClient() {
};


void TankClient::begin(){
    // Set Pinmodes

    // Motors
    pinMode(PIN_MOTOR_1_FORWARD, OUTPUT);
    pinMode(PIN_MOTOR_1_REVERSE, OUTPUT);
    pinMode(PIN_MOTOR_2_FORWARD, OUTPUT);
    pinMode(PIN_MOTOR_2_REVERSE, OUTPUT);
    pinMode(PIN_MOTOR_3_FORWARD, OUTPUT);
    pinMode(PIN_MOTOR_3_REVERSE, OUTPUT);
    pinMode(PIN_MOTOR_4_FORWARD, OUTPUT);
    pinMode(PIN_MOTOR_4_REVERSE, OUTPUT);

    // Bump Sensors
    pinMode(PIN_BUMP_FRONT, INPUT);
    pinMode(PIN_BUMP_REAR, INPUT);
    
    // Ultrasonic Sensors
    pinMode(PIN_US_FRONT_TRIG, OUTPUT);
    pinMode(PIN_US_FRONT_ECHO, INPUT);
    pinMode(PIN_US_REAR_TRIG, OUTPUT);
    pinMode(PIN_US_REAR_ECHO, INPUT);

    // Infrared Sensors
    pinMode(PIN_IR_1, INPUT);
    pinMode(PIN_IR_2, INPUT);
    pinMode(PIN_IR_3, INPUT);
    pinMode(PIN_IR_4, INPUT);
};


void TankClient::setMotorPWM(int motorId, int pwm){
    // Inputs: motorId, an int [1, 4] to select the target motor
    //         pwm, the pwm of the motor, can be + or - to indicate direction

    // Verify and correct bounds on pwm value
    if (pwm > 255) {pwm = 255;} 
    else if (pwm < -255) {pwm = -255;}

    // Verify motorId
    if (motorId < 1 || motorId > 4) {return;}

    int forward_pin;
    int reverse_pin;
    
    // Get pins to use
    if (motorId == 1) {
        forward_pin = PIN_MOTOR_1_FORWARD;
        reverse_pin = PIN_MOTOR_1_REVERSE;
    }
    else if (motorId == 2) {
        forward_pin = PIN_MOTOR_2_FORWARD;
        reverse_pin = PIN_MOTOR_2_REVERSE;
    }
    else if (motorId == 3) {
        forward_pin = PIN_MOTOR_3_FORWARD;
        reverse_pin = PIN_MOTOR_3_REVERSE;
    }
    else {
        forward_pin = PIN_MOTOR_4_FORWARD;
        reverse_pin = PIN_MOTOR_4_REVERSE;
    }

    // Set pin PWM and GND
    if (pwm >= 0) {
        analogWrite(forward_pin, abs(pwm));
        digitalWrite(reverse_pin, 0);
    }
    else {
        digitalWrite(forward_pin, 0);
        analogWrite(reverse_pin, abs(pwm));
    } 
};


// sets all motor speeds to 0
void TankClient::turnOffMotors() {
    TankClient::setMotorPWM(1, 0);
    TankClient::setMotorPWM(2, 0);
    TankClient::setMotorPWM(3, 0);
    TankClient::setMotorPWM(4, 0);
};


// sets motors 1 and 2 speeds to specified pwm
void TankClient::setLeftMotorPWM(int pwm) {
    TankClient::setMotorPWM(1, pwm);
    TankClient::setMotorPWM(2, pwm);
};


// sets motors 3 and 4 speeds to specified pwm
void TankClient::setRightMotorPWM(int pwm) {
    TankClient::setMotorPWM(3, pwm);
    TankClient::setMotorPWM(4, pwm);
};


// reads and returns distance in meters (maximum of 1) from specified ultrasonic distance sensor (only valid for sensorIds 1 or 7)
float TankClient::readDistanceSensor(int sensorId) {
    // Verify sensorId
    if (sensorId != 1 && sensorId != 7) {return -1;}

    int trig_pin;
    int echo_pin;

    // Get pin to use
    if (sensorId == 1) {
        trig_pin = PIN_US_FRONT_TRIG;
        echo_pin = PIN_US_FRONT_ECHO;
    }
    else {
        trig_pin = PIN_US_REAR_TRIG;
        echo_pin = PIN_US_REAR_ECHO;
    }

    // Trigger and read the distance sensor
    digitalWrite(trig_pin, 0);
    delayMicroseconds(2);
    digitalWrite(trig_pin, 1);
    delayMicroseconds(10);
    digitalWrite(trig_pin, 0);
    int duration = pulseIn(echo_pin, 1);
    if (duration < 20000 && duration > 0) {
        return min(duration * .0001715, 1);
    } else return -1;
};


// reads and returns the state of the specified infrared sensor (only valid for sensorIds 1 through 4)
int TankClient::readIrSensor(int sensorId){
    // Verify sensorId
    if (sensorId < 1 || sensorId > 4) {return -1;}

    int sensor_pin;

    // Get pin to use
    if (sensorId == 1){
        sensor_pin = PIN_IR_1;
    }
    else if (sensorId == 2){
        sensor_pin = PIN_IR_2;
    }
    else if (sensorId == 3){
        sensor_pin = PIN_IR_3;
    }
    else {
        sensor_pin = PIN_IR_4;
    }

    // Read and return the sensor state
    return digitalRead(sensor_pin);
};


// reads and returns the state of the specified infrared sensor (only valid for sensorIds 1 or 7)
int TankClient::readBumpSensor(int sensorId){
    // Verify sensorId
    if (sensorId != 1 && sensorId != 7) {return -1;}

    int sensor_pin;

    // Get pin to use
    if (sensorId == 1) {
        sensor_pin = PIN_BUMP_FRONT;
    }
    else {
        sensor_pin = PIN_BUMP_REAR;
    }

    // Read and return the sensor state
    return digitalRead(sensor_pin);  
};
