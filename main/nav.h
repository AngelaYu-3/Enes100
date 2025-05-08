#ifndef NAV_H
#define NAV_H

#include "sensors.h"
#include "movement.h"
#include "Enes100.h"

/*
 * Functions for navigation objective: 
 *    Objective II: navigating through obstacles
 *    Objective III: navigating through limbo
 *
 * Note: navigation Objective I is in main.ino setup()
 */

/*
 * navigating past one set of obstacles
 */
void nav_obs(double ultrasonic_thresh, double coordinate_thresh, int speed) {
  double curr_distance = ultra_get_distance();
  double curr_theta = Enes100.getTheta();
  double curr_y = Enes100.getY();
  double curr_x = Enes100.getX();
  bool detected_obstacle = false;
  bool strafe_right = false;

  // move forward until OTV sees an obstacle
  while (!detected_obstacle) {
    // Enes100.println("obstacle not detected");
    if (curr_x > 2.5) {
      Enes100.println("kicking out of entire loop");
      return;
    }

    move_forward(100);

    if (curr_distance < 13.5 + ultrasonic_thresh) {
      detected_obstacle = true;
      Enes100.println("obstacle detected");
      stop_motors();
    }
    curr_distance = ultra_get_distance();
    curr_x = Enes100.getX();
  }

  // decide on an initial strafing direction
  strafe_right = true;

  curr_y = Enes100.getY();
  set_angle_simple(0, 0.08);
  curr_distance = ultra_get_distance(); 
  // while an obstacle is still seen either strafe right or left
  while (curr_distance < 13.5 + ultrasonic_thresh) {
    // break out of while loop if nothing is seen
    if (curr_distance > 30) {
      Enes100.println("nothing seen!");
      break;
    }

    // decide which way to go depending on obstacle configuration
    curr_y = Enes100.getY();
    // curr_distance = get_ultra_distance();
    if (curr_y <= 0.35) {
        // obstacle right and middle
        strafe_right = false;
        // stop_motors();
        // set_angle_simple(0, 0.05);
    }

    if (curr_y >= 1.3) {
        strafe_right = true;
        // stop_motors();
        // set_angle_simple(0, 0.05);
    }
    // if (curr_distance < 13.5 + ultrasonic_thresh) {

    // }

    // checking and fixing alignment + angle issues
    // curr_distance = ultra_get_distance();
    // if (curr_distance < 6) {
    //   stop_motors();
    //   Enes100.println("FIXING ALIGNMENT");
    //   move_to_dist_back(20, coordinate_thresh, 100);
    //   Enes100.println("DONE FIXING ALIGNMENT");
    // }
    // curr_theta = Enes100.getTheta();
    // if (curr_theta < -0.05 || curr_theta > 0.05) {
    //   stop_motors();
    //   Enes100.println("FIXING ANGLE");
    //   set_angle_simple(0, 0.05);
    //   Enes100.println("DONE FIXING ANGLE");
    // }

    // strafe in correct direction
    if (strafe_right) {
      shift_right(speed);
    } else {
      shift_left(speed);
    }

    // Enes100.println("in while loop!");

    // update sensor values
    // re_oriente();
    curr_distance = ultra_get_distance();
    curr_y = Enes100.getY();
    curr_theta = Enes100.getTheta();
  }

  Enes100.println("OUT OF WHILE LOOP");

  // strafe a little more to clear robot body
  for (int i = 0; i < 2.7; i++) {
    if (strafe_right) {
      shift_right(speed);
    } else {
      shift_left(speed);
    }
    delay(500);
  }
  Enes100.println("FINIISHED NAVIGATING OBSTACLE");
}

/*
 * completing limbo
 */
void limbo() {
  set_angle_simple(0, 0.05);
  nav_x(150, 2.6, true);
  nav_y(150, 1.6);
  move_to_dist_for(13.5, 0.05, 100);
}

#endif