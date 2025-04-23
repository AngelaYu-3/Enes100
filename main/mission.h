#ifndef MISSION_H
#define MISSION_H

#include "sensors.h"
#include <VarSpeedServo.h>
#include <Arduino.h>
#include "nav.h"

const int servo_pin = 11;
VarSpeedServo myServo;

void arm_setup() {
  myServo.attach(servo_pin);

  // Initialize the arm to starting position (parallel to floor)
  myServo.write(0);
  delay(1000); // Give time for the servo to reach position
}

// Move arm from parallel to perpendicular
void arm_up() {
  myServo.slowmove(90, 100);  // Move to 90 degrees at moderate speed
  delay(1000);  // Wait for movement to complete
}

// Move arm from perpendicular back to parallel
void arm_down() {
  myServo.slowmove(0, 30);  // Move to 0 degrees at moderate speed
  delay(1000);  // Wait for movement to complete
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