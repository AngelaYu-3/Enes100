#include "movement.h"
#include "sensors.h"
#include "mission.h"
#include "Enes100.h"

bool is_mission_done = false;

void setup() {
  // put your setup code here, to run once:
  delay(500);
  Serial.begin(9600);
  // Enes100.begin("Space Crash", CRASH_SITE, 734, 1116, 13, 12);
  motor_setup();
  /*ultra_setup();
  color_setup();
  arm_setup();
  // pinMode(7, OUTPUT);
  pinMode(enA, OUTPUT);
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  pinMode(enB, OUTPUT);*/
}

void loop() {
  // digitalWrite(7, HIGH);
  // move_arm(20);
  // *** wifi test ***
  // testing wifi receive
  
  /*wifi_get_X();
  wifi_get_Y();
  wifi_get_theta();
  // testing wifi transmit
  wifi_transmit_length(100);
  wifi_transmit_height(100);*/


  // **** ultrasonic test ****
  /*ultra_get_distance();
  delay(1000);*/


  // **** color test ***
  /*is_red();
  delay(100);*/


  // *** motor test ***
  // move_forward(100);
  
  // move_forward(100);
  Serial.println("hello");
  control_motor_A(100, true);
  // digitalWrite(in1, LOW); // low
  // digitalWrite(in2, HIGH);
  control_motor_B(100, true); 
  control_motor_C(100, true); 
  control_motor_D(100, true); 
  // delay(2000);
  //stop_motors();
  // delay(2000);
  // delay(1000);
  // control_motor_A(100, false);
  // delay(1000);
  // stop_motor_A();*/
}
