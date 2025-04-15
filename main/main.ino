#include "movement.h"
#include "Enes100.h"
#include "nav.h"

bool is_mission_done = false;

void setup() {
  // put your setup code here, to run once:
  delay(500);
  Serial.begin(9600);
  // Enes100.begin("Space Crash", CRASH_SITE, 734, 1116, 13, 12);
  // ultra_setup();
  // color_setup();
  motor_setup();

  // Always start at bottom obstacles
  // navigatingCoorY(250, bottom_obstacle);
}

void loop() {
  // *** wifi test ***
  // testing wifi receive
  /*
  wifi_get_X();
  wifi_get_Y();
  wifi_get_theta();
  // testing wifi transmit
  wifi_transmit_length(100);
  wifi_transmit_height(100);*/


  // **** ultrasonic test ****
  // ultra_get_distance();
  // delay(1000);


  // **** color test ***
  // is_red();
  // delay(100);
  // color_setup();
  // put your main code here, to run repeatedly:


  // *** motor test ***
  Serial.println("hello");
  control_motor_A(100, true);
  control_motor_B(100, true); 
  control_motor_C(100, true); 
  control_motor_D(100, true); 
  // delay(2000);
  //stop_motors();
  // delay(2000);
  // delay(1000);
  // control_motor_A(100, false);
  // delay(1000);
  // stop_motor_A();

  // **** navigation code ****
  // Read and print sensor data on every loop iteration
    Enes100.print("Ultrasonic Reading: ");
    Enes100.println(Tank.readDistanceSensor(1));
    
    Enes100.print("X = "); 
    Enes100.println(Enes100.getX());
    
    Enes100.print("Y = "); 
    Enes100.println(Enes100.getY());
    
    Enes100.print("Theta = "); 
    Enes100.println(Enes100.getTheta());
    
    // Complete mission, currently hard coded to always be true
    if (!is_mission_done) {
        mission();
    } else if (!is_obstacle_done) {
    // The code for obstacles is considered finished after we pass the second obstacle
        nav.navigatingAroundObstacle(250);
    } else if (!is_limbo_done) {   
        nav.limbo(250);
    } else {
        movement.stop_motors();
    }
    
    delay(100); 
}