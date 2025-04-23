#include "movement.h"
#include "sensors.h"
#include "mission.h"
#include "Enes100.h"

#include <Servo.h>
Servo myservo;
int pos = 0;

bool is_mission_done = false;

void setup() {
  // put your setup code here, to run once:
  delay(500);
  Serial.begin(9600);
  // color_setup();
  // Serial.println("before wifi");
  // Enes100.begin("Space Crash", CRASH_SITE, 734, 1116, 13, 12);
  // Serial.println("after wifi");
  myservo.attach(11);
  // motor_setup();
  // arm_setup();
  // arm_setup();
  // ultra_setup();
  // digitalWrite(11, OUTPUT);
  // digitalWrite(13, OUTPUT);
  
  // pinMode(7, OUTPUT);
  // pinMode(enA, OUTPUT);
  // digitalWrite(in1, LOW);
  // digitalWrite(in2, LOW);
  // pinMode(enB, OUTPUT);
}

void loop() {
  // digitalWrite(11, HIGH);
  // digitalWrite(13, HIGH);
  // move_to_dist(14, 0);
  Serial.println("hello");
  // move_forward(100);

  // *** arm test ***
  for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(100);                       // waits 15ms for the servo to reach the position
  }
  for (pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(100);                       // waits 15ms for the servo to reach the position
  }

  // *** wifi test ***
  /* wifi_get_X();
  wifi_get_Y();
  wifi_get_theta();
  wifi_transmit_length(100);
  wifi_transmit_height(100);*/


  // **** ultrasonic test ****
  //ultra_get_distance();
  // delay(1000);

  // **** color test ***
  // is_red();
  // delay(1000);

}
