#ifndef NAV_H
#define NAV_H

#include "sensors.h"
#include "movement.h"
#include "Enes100.h"

const double first_obs_x = 0;
const double second_obs_x = 0;
const double limbo_x = 0;
const double limbo_y = 0;

const double pos_1 = 0;
const double pos_2 = 0;
const double pos_3 = 0;

const double mid_y = 1;

/*
 * navigating past one set of obstacles
 */
void nav_obs(double thresh, double coordinateThresh) {
  int currentDistance = ultra_get_distance();
    while (currentDistance <= 15 - thresh) {
      if (Enes100.getY() > 1 + coordinateThresh) {
        shift_right(80);
      } else {
        shift_left(80);
      }
      currentDistance = ultra_get_distance();
    }
    stop_motors();
}

/*
 * navigating past all obstacles and completing limbo
 */
void nav_all_obs() {
  // initial angle setup and move
  set_angle_simple(0, 0);
  move_to_dist(100, 15, 0);

  // need to go past both first and second obstacle
  if (Enes100.getX() <= first_obs_x) {
    nav_obs(0.8);

    while (Enes100.getX() < second_obs_x) {
      move_forward(100);
      if (ultra_get_distance() <= 15) {
        stop_motors();
        nav_obs(0.8);
      } 

      if (Enes100.getX() > (second_obs_x + 0.5)) {
        break;
      }
    }
  } // only need to go past second obstacle
  else (Enes100.getX() <= second_obs_x) {
    nav_obs(0.8);
  }

  // navigated through all obstacles and complete limbo
  move_forward(100);
  if (Enes100.getX() >= limbo_x) {
    stop_motors();
    nav_y(100, limbo_y);
    move_to_dist(15, 0);
  }
}


//over here, i'm testing nav from point of mission, assuming mission is already completed. as such, 
//i'm starting from roughly where the mission site is (can be at any angle, as it's resetting to zero)
const int no_more_obstacles = 0;
const int end_pos_x = 0;
const double coordThresh = 0.05;
const double thresh_ultrasonic = 0.5;
void getMeHome() {
  setAngle_test(0, 0.05);

  //will keep trying to see if obstacles exist until there are no more obstacles (we can get position by measuring)
  while (Enes100.getX() < no_more_obstacles - coordThresh) {
    int currentDistance = ultra_get_distance();
    while (currentDistance <= 15 - thresh_ultrasonic) {
      if (Enes100.getY() > 1 + coordThresh) {
        shift_right(80);
      } else {
        shift_left(80);
      }
      currentDistance = ultra_get_distance();

    }

    for (int i = 0; i < 20; i++) {
      move_forward(100);
      delay(100);
      if (ultra_get_distance() <= 15 - thresh_ultrasonic) {
        break; // stop mid-move if obstacle appears
      }
    }
    setAngle_test(0, 0.05);
    // moves forward, going back to angle of 0 every 2 sec (delay can be adjusted)
    Enes100.print("Current X: ");
    Enes100.print(Enes100.getX());
    Enes100.print("  Y: ");
    Enes100.print(Enes100.getY());
    }
  



  //executes after
  while (Enes100.getX() > no_more_obstacles + coordThresh && Enes100.getX() < end_pos_x - coordThresh) {
    if (Enes100.getY() < limbo_y - coordThresh) {
      shift_left(100);
    } else if (Enes100.getY() > limbo_y + coordThresh) {
      shift_right(100);
    }

    //SENDING ITTTTTTTT!!!! (we can change it later if it doesn't work lol)
    move_to_dist(250, 15, 0.5);
  }



  if (Enes100.getX() >= end_pos_x + coordThresh) {
    Enes100.println("Raj has arrived...")
    stop();
}


// const double first_obstacle_xcoor = 1.7;
// const double second_obstacle_xcoor = 2.5;
// const double bottom_obstacle = 0.5;
// const double middle_obstacle = 1.0;
// const double top_obstacle = 1.5;
// const double limbo_start_coor = 1.5;

// void obstacle_setup() {
//   // Always start at bottom obstacles
//   navigatingCoorY(250, bottom_obstacle);
// }

// // This function contains the logic behind the avoiding obstacles mission. 
// void navigatingAroundObstacle(double pwm) {
//     // Enes100.println("Navigating around obstacles");
//     double currentX = Enes100.getX();
//     double currentY = Enes100.getY();
//     double ultrasonicReading;
    
//     // Check if we've passed the second obstacle
//     if (currentX > second_obstacle_xcoor) {
//         is_obstacle_done = true;
//         // Enes100.println("Obstacles cleared");
//         return;
//     }
    
//     // Always make sure we're facing forward
//     set_angle(0, 0.07, 50);
    
//     // Get current ultrasonic reading
//     ultrasonicReading = ultra_get_distance();
    
//     // Specific handling for first obstacle column
//     if (abs(currentX - first_obstacle_xcoor) < 0.1) {
//         // We're at the first obstacle column - force a stop and check
//         stop_motors(); 
//         delay(100);  
        
//         // Make sure we're facing forward before checking
//         set_angle(0, 0.07, 50);
//         ultrasonicReading = ultra_get_distance();

//         // Check if there's an obstacle
//         if (ultrasonicReading != -1 && ultrasonicReading < 20) {
//             // If not at the top, move up by 0.5
//             if (currentY < top_obstacle) {
//                 // Enes100.println("Obstacle at first column, moving up");
//                 navigatingCoorY(pwm, currentY + 0.5);
//             } else {
//                 // Enes100.println("At top of first column, moving down");
//                 navigatingCoorY(pwm, currentY - 0.5);
//             }
//         } else {
//             // No obstacle at this Y position, continue forward
//             // Enes100.println("No obstacle detected at first column, continuing");
//             move_forward(pwm);
//             delay(150);
//         }
//     } 
//     // Specific handling for second obstacle column
//     else if (abs(currentX - second_obstacle_xcoor) < 0.1) {
//         // We're at the second obstacle column - force a stop and check
//         stop_motors(); 
//         delay(100);  
        
//         // Make sure we're facing forward before checking
//         set_angle(0, 0.07, 50);
//         ultrasonicReading = ultra_get_distance();
        
//         // Check if there's an obstacle
//         if (ultrasonicReading != -1 && ultrasonicReading < 20) {
//             // If not at the top, move up by 0.5
//             if (currentY < top_obstacle) {
//                 // Enes100.println("Obstacle at second column, moving up");
//                 navigatingCoorY(pwm, currentY + 0.5);
//             } else {
//                 // Enes100.println("At top of second column, moving down");
//                 navigatingCoorY(pwm, currentY - 0.5);
//             }
//         } else {
//             // No obstacle at this Y position, continue forward
//             // Enes100.println("No obstacle detected at second column, continuing");
//             move_forward(pwm);
//             delay(100);
//         }
//     }
//     // Standard obstacle detection during general movement
//     else if (ultrasonicReading != -1 && ultrasonicReading < 20) {
//         stop_motors(); 
        
//         // If not at the top, move up by 0.5
//         if (currentY < top_obstacle) {
//             // Enes100.println("Obstacle detected, moving up by 0.5");
//             navigatingCoorY(pwm, currentY + 0.5);
//         } 
//         // If at the top, start moving down by 0.5
//         else {
//             // Enes100.println("At top, moving down by 0.5");
//             navigatingCoorY(pwm, currentY - 0.5);
//         }
//     } 
//     // No obstacle detected, continue moving forward with speed adjustment
//     else {
//         // Slow down when approaching known obstacle columns
//         if ((currentX < first_obstacle_xcoor && currentX > first_obstacle_xcoor - 0.2) || 
//             (currentX < second_obstacle_xcoor && currentX > second_obstacle_xcoor - 0.2)) {
//             move_forward(pwm * 0.5); // Move slower near obstacle columns
//         } else {
//             // Enes100.println("Path clear, moving forward");
//             move_forward(pwm);
//         }
//         delay(100);
//     }
    
//     // Check if we've passed the second obstacle
//     currentX = wifi_get_X();
//     if (currentX > second_obstacle_xcoor) {
//         is_obstacle_done = true;
//         // Enes100.println("Obstacles cleared!");
//     }
// }

#endif