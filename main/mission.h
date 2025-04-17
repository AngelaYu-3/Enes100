#include "sensors.h"
#include "nav.h"

const double small_length = 135.0;  // The smaller possible length in mm
const double large_length = 180.0;  // The larger possible length in mm
const double half_small_length = 67.5;  // Half of the smaller length
const double height = 270.0; 
const double centerOfSite;

void mission_setup() {

}

void measureAnnomoly(double pwm) {
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