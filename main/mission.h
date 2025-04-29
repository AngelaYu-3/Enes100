#ifndef MISSION_H
#define MISSION_H

#include "sensors.h"
#include "movement.h"
#include "Enes100.h"
#include <Servo.h>
#include <Arduino.h>

const int servo_pin = 11;
Servo myServo;

// Constants for measure_anomoly
const double small_length = 135.0;  // The smaller possible length in mm
const double large_length = 180.0;  // The larger possible length in mm
const double half_small_length = 67.5;  // Half of the smaller length
const double height = 270.0; 
const double centerOfSite;

// constants for finding crash site
const double site_A_x = 0;
const double site_A_y = 0;
const double site_B_x = 0;
const double site_B_y = 0;

// x = 0.3 y = 1

void arm_setup() {
  myServo.attach(servo_pin);

  // Initialize the arm to starting position (parallel to floor)
  myServo.write(0);
  delay(1000); // Give time for the servo to reach position
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

void initial_setup() {
  if (Enes100.getY() < 1) {
    set_angle_simple(PI/2, 0);
  } else {
    set_angle_simple(-(PI/2), 0);
  }
  
  move_to_dist(13.5, 0);
}

void measure_anomoly() {
  double height = 0.024;
  double length1 = 0.018;
  double length2 = 0.0135;

  double initial_x = Enes100.getX();
  double final_x = 0;
  double length;

  double dist = ultra_get_distance();

  while (dist < 20) {
    shift_right(80);
    dist = ultra_get_distance();
  }

  stop_motors();
  final_x = Enes100.getX();
  length = 2 * abs(final_x - initial_x);

  if (abs(length1 - length) < abs(length2 - length)) {
    length = length1;
  } else {
    length = length2;
  }

  Enes100.println("final x: ");
  Enes100.println(final_x);
  wifi_transmit_height(height);
  wifi_transmit_length(length);
  delay(1000);
}

void find_anomoly() {
  if (is_red()) {
    stop_motors();
  } else {
    nav_y(100, 1);
    if (Enes100.getX() < 1) {
      nav_x(100, 0.1, true);
    } else {
      nav_x(100, 1.3, true);
    }
    set_angle_simple(3, 0);
    move_to_dist(13.5, 0);
  }
}

// bool find_anomoly() {
//   if (is_red()) {
//     return true;
//   } else if (check_site_A()){
//     while (!is_red()) {
//       // back out
//       navigatingCoorY(100, 1.30);
//       // rotate
//       // move forward
//       // check is red
//       // if red stop return true
//       // else continue in while loop
//     }
//   } else {
//     while (!is_red()) {
//       navigatingCoorY(100, 0.60);
//     }
//   }
// }

// // void measure_anomoly(double pwm) {
// //   // move to x = center of block
// //   navigatingCoorX(pwm, centerOfSite);
// //   stop_motors();
// //   delay(100);

// //   // strafe left and continuously update the variable until the color sensor.
// //   double length = small_length;   // Assume length is smaller length first.
// //   double strafeDistance = 0.0;
//   double startY = wifi_get_Y();
//   bool detectedRed = false;
//   bool measurementComplete = false;

//   shift_left(pwm);

//   while (!measurementComplete) {
//     // Get current position to calculate distance moved
//     double currentY = wifi_get_Y();
//     strafeDistance = abs(currentY - startY) * 1000.0;  // Convert to mm
    
//     // Check if the color sensor detects red
//     if (is_red()) {
//       detectedRed = true;
//     }

//     // If we were detecting red but no longer are, assume its smaller length, don't change the length value.
//     if (detectedRed && !is_red()) {
//       measurementComplete = true;
//     }
    
//     // If we've strafed more than half of the small length and still detecting red, assume it's the larger length, change the length value;
//     if (strafeDistance > half_small_length && is_red()) {
//       length = large_length;
//       measurementComplete = true;
//     }
//   }
    
//   delay(10);  
    
//   stop_motors();
//   wifi_transmit_length(length);
//   wifi_transmit_height(height);
// }

// void fix_anomoly() {

// }

#endif