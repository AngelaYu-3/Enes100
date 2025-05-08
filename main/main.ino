#include "movement.h"
#include "sensors.h"
#include "mission.h"
#include "Enes100.h"
#include "nav.h"
#include <Servo.h>

// const int servo_pin = 11;
// Servo my_servo;
// int pos = 0;

// void setServoAngle(int angle) {
//   // Convert angle to pulse width (1000-2000 microseconds)
//   int pulseWidth = map(angle, 0, 180, 1000, 2000);
  
//   // Generate the 50Hz PWM signal (20ms period)
//   digitalWrite(servo_pin, HIGH);
//   delayMicroseconds(pulseWidth);
//   digitalWrite(servo_pin, LOW);
//   delayMicroseconds(20000 - pulseWidth);
// }

double curr_y;
bool is_flap_red = false;
// **** setup, runs once ****
void setup() {
  // delay(500);
  // Serial.begin(9600);

  // connecting to WiFi module
  Enes100.begin("Space Crash", CRASH_SITE, 894, 1116, 51, 50); // 1116
  Enes100.println("Connected...");
  delay(500);

  // // motor and sensor setups
  motor_setup();
  ultra_setup();
  color_setup();

  // servo setup
  // my_servo.attach(servo_pin);
  // my_servo.write(0);
  // delay(1000); // Give time for the servo to reach position

  // *** Nav Objective I: move to within 150mm of crash site ***
  Enes100.println("**** Starting Nav Objective I ****");
  if (Enes100.getY() > 1) {
    set_angle_simple(-(PI/2), 0.05);
  } else {
    set_angle_simple((PI/2), 0.05);
  }

  move_to_dist_for(13.5, 0, 80);
  curr_y = Enes100.getY();
  // if (curr_y > 1 || curr_y == -1) {
  //   is_flap_red = true;
  //   move_to_dist_for(13.5, 0, 60);
  // }
  stop_motors();
  
  Enes100.println("**** Finished Nav Objective I ****");
  delay(200);

  // // *** Mission Objective I: find anomoly
  Enes100.println("**** Starting Mission Objective I ****");
  if (is_red()) {
    Enes100.println("This side is red");
    is_flap_red = true;
  } else {
    Enes100.println("This side is NOT red");
  }
  
  // delay(200);
  // find_anomoly();
  Enes100.println("**** Finished Mission Objective I ****");

  // *** Mission Objective II: measure anomolly
  Enes100.println("**** Starting Mission Objective II ****");
  measure_anomoly(is_flap_red);
  delay(200);
  Enes100.println("**** Finished Mission Objective II ****");

  // *** Nav Objective II: navigate through obstacles
  Enes100.println("**** Starting Nav Objective II ****");
  double curr_x;
  move_to_dist_back(20, 0.05, 80);
  set_angle_simple(0, 0.05);
  nav_y(100, 1);
  nav_x(100, 0.5, true);

  curr_x = Enes100.getX();
  // nav_obs(0.05, 0.05, 100);
  // nav_obs(0.05, 0.05, 100);
  while (curr_x <= 2.4 || curr_x == -1) {
    curr_x = Enes100.getX();
    nav_obs(0.05, 0.05, 100);
  }

  Enes100.println("**** Finished Nav Objective II ****");
  curr_x = Enes100.getX();
  // *** Nav Objective III: navigate through limbo
  if (curr_x >= 2.45) {
    Enes100.println("**** Starting Nav Objective III ****");
    limbo();
    Enes100.println("**** Finished Nav Objective III ****");
  }
}


// ****** 20 ms or 50 Hz (On 1 MS, Off ) ******

// **** loop, continuously runs ****
void loop() {
  // Serial.println(ultra_get_distance());
  // shift_right(100);
  // Enes100.println(is_red());
  // move_forward(100);
  // control_motor_C(100, true);
  // move_forward(100);
  // Enes100.println(ultra_get_distance());
  // is_red();
  // digitalWrite(13, HIGH);
  // delay(1000);
  // digitalWrite(13, LOW);
  // delay(1000);
  // Enes100.println(ultra_get_distance());
  // move_forward(100);
  // Enes100.println(is_red());
  // delay(1000);
  // shift_left(150);
  // Enes100.println(ultra_get_distance());
  // delay(1000);
  // // measure_anomoly();

  // *** SERVO ARM TEST ***
  // for (int angle = 0; angle <= 180; angle += 10) {
  //   for (int pulse = 0; pulse < 50; pulse++) {  // Hold position for ~1 second
  //     setServoAngle(angle);
  //   }
  // }
  // 
  // my_servo.write(180);
  // delay(1000);
  // my_servo.write(90);
  // delay(1000);

  // *** WIFI TESTt ***
  // delay(1000);
  // wifi_get_X();
  // wifi_get_Y();
  // wifi_get_theta();


}
