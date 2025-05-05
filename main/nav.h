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
  double curr_y = Enes100.getY();
  bool detected_obstacle = false;
  bool strafe_right = false;

  // move forward until OTV sees an obstacle
  while (!detected_obstacle) {
    Enes100.println("obstacle not detected");
    move_forward(100);
    if (curr_distance <= 15 - ultrasonic_thresh) {
      detected_obstacle = true;
      Enes100.println("OBSTACLE DETECTED");
    }
    curr_distance = ultra_get_distance();
  }

  // decide on an initial strafing direction
  if (Enes100.getY() > 1 + coordinate_thresh) {
    strafe_right = true;
  } else {
    strafe_right = false;
  }

  curr_y = Enes100.getY();

  // while an obstacle is still seen either strafe right or left
  while (curr_distance < curr_distance < 15 + coordinate_thresh) {
    // break out of while loop if nothing is seen
    if (curr_distance > 30) {
      break;
    }

    // decide which way to go depending on obstacle configuration
    curr_y = Enes100.getY();
    if (curr_distance < 20) {
      if (curr_y >= 1.7) {
        // obstacle left and middle
        Enes100.println("in first if statement");
        strafe_right = true;
      } else if (curr_y <= 0.3) {
        // obstacle right and middle
        strafe_right = false;
        Enes100.println("in second if statement");
      }
    }

    // strafe in correct direction
    if (strafe_right) {
      shift_right(speed);
    } else {
      shift_left(speed);
    }

    Enes100.println("in while loop!");

    // update sensor values
    curr_distance = ultra_get_distance();
    Enes100.println(curr_distance);
    curr_y = Enes100.getY();
  }

  Enes100.println("OUT OF WHILE LOOP");

  // strafe a little more to clear robot body
  for (int i = 0; i < 2.5; i++) {
    if (strafe_right) {
      shift_right(speed);
    } else {
      shift_left(speed);
    }
    delay(500);
  }

  stop_motors();
}

/*
 * completing limbo
 */
void limbo() {
  set_angle_simple(0, 0.05);
  nav_x(150, 2.8, true);
  nav_y(150, 1.3);
  move_to_dist_for(13.5, 0.05, 200);
}

#endif