#include "movement.h"
#include "sensors.h"
#include "mission.h"
#include "Enes100.h"
#include "nav.h"
#include <Servo.h>

const int servo_pin = 11;
Servo myservo;
int pos = 0;
void setup() {
  // put your setup code here, to run once:
  // delay(500);
  // Serial.begin(9600);
  Enes100.begin("Space Crash", CRASH_SITE, 894, 1116, 50, 51);
  Enes100.println("Connected...");
  delay(500);
  motor_setup();

  myservo.attach(servo_pin);
  myservo.write(0);
  delay(1000); // Give time for the servo to reach position


  // Initialize the arm to starting position (parallel to floor)
  
  ultra_setup();
  // color_setup();
  // set_angle_simple(0, 0.05);
  // nav_obs(0.05, 0.08, 100);

  // if (Enes100.getY() > 1) {
  //   set_angle_simple(-(PI/2), 0.05);
  // } else {
  //   set_angle_simple(PI/2, 0.05);
  // }
  
  // move_to_dist(13.5, 0);


}

bool set_angle = false;
// 20 ms or 50 Hz
void loop() {
  // Enes100.println(ultra_get_distance());
  // move_forward(100);
  // Enes100.println(is_red());
  // delay(1000);
  // measure_anomoly();

  // *** WIFI TESTt ***
  // delay(1000);
  // wifi_get_X();
  // wifi_get_Y();
  // wifi_get_theta();


  // *** arm test ***
  myservo.write(180);
  delay(1000);
  myservo.write(90);
  delay(1000);
}
