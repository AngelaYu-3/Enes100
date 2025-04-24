#ifndef MISSION_H
#define MISSION_H

#include "sensors.h"
#include "movement.h"
#include <Servo.h>
#include <Arduino.h>


const int servo_pin = 11;
Servo myServo;

void arm_setup() {
  myServo.attach(servo_pin);
}

void move_arm() {
  int pos = 0;
  for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    myServo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
  for (pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
    myServo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
  // delay(1000);
}

bool check_site_A() {
  setAngle(90, 5, 100);
  navigatingCoorX(100, 0.55);
  navigatingCoorY(100, 1.40);

  if (ultra_get_distance() != 0) {
    return true;
  }

  return false;
}

void check_site_B() {
  setAngle(-90, 5, 100);
  navigatingCoorX(100, 0.55);
  navigatingCoorY(100, 0.50);
}

bool find_anomoly() {
  if (is_red()) {
    return true;
  } else if (check_site_A()){
    while (!is_red()) {
      // back out
      navigatingCoorY(100, 1.30);
      // rotate
      // move forward
      // check is red
      // if red stop return true
      // else continue in while loop
    }
  } else {
    while (!is_red()) {
      navigatingCoorY(100, 0.60);
    }
  }
}

void measure_anomoly() {
  wifi_transmit_height(270);
}

void fix_anomoly() {

}

void mission() {
  // check site A
  if (check_site_A()) {
    find_anomoly();
    measure_anomoly();
    fix_anomoly();
  } else {
    check_site_B();
    find_anomoly();
    measure_anomoly();
    fix_anomoly();
  }
}

#endif