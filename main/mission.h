#include "sensors.h"
#include "movement.h"
#include <VarSpeedServo.h>
#include "nav.h"

const int servo_pin = 11;
VarSpeedServo myServo;

// Constants for measure_anomoly
const double small_length = 135.0;  // The smaller possible length in mm
const double large_length = 180.0;  // The larger possible length in mm
const double half_small_length = 67.5;  // Half of the smaller length
const double height = 270.0; 
const double centerOfSite;

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

void arm_setup() {
  myServo.attach(servo_pin);
}

void move_arm(int degree, int speed) {
  myServo.write(45, 10, true);
  delay(1000);
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

void measure_anomoly(double pwm) {
  // move to x = center of block
  navigatingCoorX(pwm, centerOfSite);
  stop_motors();
  delay(100);

  // strafe left and continuously update the variable until the color sensor.
  double length = small_length;   // Assume length is smaller length first.
  double strafeDistance = 0.0;
  double startY = wifi_get_Y();
  bool detectedRed = false;
  bool measurementComplete = false;

  shiftLeft(pwm);

  while (!measurementComplete) {
    // Get current position to calculate distance moved
    double currentY = wifi_get_Y();
    strafeDistance = abs(currentY - startY) * 1000.0;  // Convert to mm
    
    // Check if the color sensor detects red
    if (is_red()) {
      detectedRed = true;
    }

    // If we were detecting red but no longer are, assume its smaller length, don't change the length value.
    if (detectedRed && !is_red()) {
      measurementComplete = true;
    }
    
    // If we've strafed more than half of the small length and still detecting red, assume it's the larger length, change the length value;
    if (strafeDistance > half_small_length && is_red()) {
      length = large_length;
      measurementComplete = true;
    }
  }
    
  delay(10);  
    
  stop_motors();
  wifi_transmit_length(length);
  wifi_transmit_height(height);
}

void fix_anomoly() {

}