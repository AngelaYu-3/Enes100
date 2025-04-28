#include "movement.h"
#include "sensors.h"
#include "mission.h"
#include "Enes100.h"
#include <Servo.h>

// Servo myservo;
// int pos = 0;

//some test

bool is_mission_done = false;

void setup() {
  // put your setup code here, to run once:
  // delay(500);
  // Serial.begin(9600);
  Enes100.begin("Space Crash", CRASH_SITE, 894, 1116, 50, 51);
  Enes100.println("Connected...");
  // motor_setup();
  // arm_setup();
  // ultra_setup();
  color_setup();
}

bool set_angle = false;
void loop() {

  measure_anomoly();
  // set_angle_simple(-(PI/2), 0);
  // nav_x(100, 1, true); moving forward until x = 100
  // nav_y(100, 0.5, false); // straying right until y = 0.5
  // initial_setup();
  
  // if (Enes100.getTheta() < 0.3 && Enes100.getTheta() > -0.3) {
  //     move_forward(100);
  // } else {
  //     set_angle_simple(0, 0.08);
  // }

  // Enes100.println(Enes100.getTheta());

  // move_to_dist(13.5, 0);
  
  // *** WIFI TESTt ***
  // delay(1000);
  // wifi_get_X();
  // wifi_get_Y();
  // wifi_get_theta();

  // **** COLOR TEST ***
  // is_red();
  // delay(1000);


  // *** arm test ***
  // myservo.write(90);
  // for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
  //   // in steps of 1 degree
  //   myservo.write(pos);              // tell servo to go to position in variable 'pos'
  //   delay(100);                       // waits 15ms for the servo to reach the position
  // }
  // for (pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
  //   myservo.write(pos);              // tell servo to go to position in variable 'pos'
  //   delay(100);                       // waits 15ms for the servo to reach the position
  // }





  // turn_right(100);

  // static bool mission_complete = false;
  
  // if (!mission_complete) {
    
  //   // navigatingCoorX(0.55, 0.05); // Assuming 0.05 is your position threshold
    
  //   // set_angle(-PI/2, 0.09, 100);
    
  //   // move_to_dist(14, 2); // Using threshold of 2cm for more stability
    
  //   mission_complete = true; // Set flag to prevent repeated execution
  // } else {
  //   stop_motors();
  // }

  // **** ultrasonic test ****
  //ultra_get_distance();
  // delay(1000);

}
