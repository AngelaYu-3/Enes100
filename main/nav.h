#ifndef NAV_H
#define NAV_H

#include "sensors.h"
#include "Enes100.h"
#include "nav.h"
#include "movement.h"

const double first_obstacle_xcoor = 1.7;
const double second_obstacle_xcoor = 2.5;
const double bottom_obstacle = 0.5;
const double middle_obstacle = 1.0;
const double top_obstacle = 1.5;
const double limbo_start_coor = 1.5;

void obstacle_setup() {
  // Always start at bottom obstacles
  navigatingCoorY(250, bottom_obstacle);
}

// This function contains the logic behind the avoiding obstacles mission. 
void navigatingAroundObstacle(double pwm) {
    // Enes100.println("Navigating around obstacles");
    double currentX = wifi_get_X();
    double currentY = wifi_get_Y();
    double ultrasonicReading;
    
    // Check if we've passed the second obstacle
    if (currentX > second_obstacle_xcoor) {
        is_obstacle_done = true;
        // Enes100.println("Obstacles cleared");
        return;
    }
    
    // Always make sure we're facing forward
    set_angle(0, 0.07, 50);
    
    // Get current ultrasonic reading
    ultrasonicReading = ultra_get_distance();
    
    // Specific handling for first obstacle column
    if (abs(currentX - first_obstacle_xcoor) < 0.1) {
        // We're at the first obstacle column - force a stop and check
        stop_motors(); 
        delay(100);  
        
        // Make sure we're facing forward before checking
        set_angle(0, 0.07, 50);
        ultrasonicReading = ultra_get_distance();

        // Check if there's an obstacle
        if (ultrasonicReading != -1 && ultrasonicReading < 20) {
            // If not at the top, move up by 0.5
            if (currentY < top_obstacle) {
                // Enes100.println("Obstacle at first column, moving up");
                navigatingCoorY(pwm, currentY + 0.5);
            } else {
                // Enes100.println("At top of first column, moving down");
                navigatingCoorY(pwm, currentY - 0.5);
            }
        } else {
            // No obstacle at this Y position, continue forward
            // Enes100.println("No obstacle detected at first column, continuing");
            move_forward(pwm);
            delay(150);
        }
    } 
    // Specific handling for second obstacle column
    else if (abs(currentX - second_obstacle_xcoor) < 0.1) {
        // We're at the second obstacle column - force a stop and check
        stop_motors(); 
        delay(100);  
        
        // Make sure we're facing forward before checking
        set_angle(0, 0.07, 50);
        ultrasonicReading = ultra_get_distance();
        
        // Check if there's an obstacle
        if (ultrasonicReading != -1 && ultrasonicReading < 20) {
            // If not at the top, move up by 0.5
            if (currentY < top_obstacle) {
                // Enes100.println("Obstacle at second column, moving up");
                navigatingCoorY(pwm, currentY + 0.5);
            } else {
                // Enes100.println("At top of second column, moving down");
                navigatingCoorY(pwm, currentY - 0.5);
            }
        } else {
            // No obstacle at this Y position, continue forward
            // Enes100.println("No obstacle detected at second column, continuing");
            move_forward(pwm);
            delay(100);
        }
    }
    // Standard obstacle detection during general movement
    else if (ultrasonicReading != -1 && ultrasonicReading < 20) {
        stop_motors(); 
        
        // If not at the top, move up by 0.5
        if (currentY < top_obstacle) {
            // Enes100.println("Obstacle detected, moving up by 0.5");
            navigatingCoorY(pwm, currentY + 0.5);
        } 
        // If at the top, start moving down by 0.5
        else {
            // Enes100.println("At top, moving down by 0.5");
            navigatingCoorY(pwm, currentY - 0.5);
        }
    } 
    // No obstacle detected, continue moving forward with speed adjustment
    else {
        // Slow down when approaching known obstacle columns
        if ((currentX < first_obstacle_xcoor && currentX > first_obstacle_xcoor - 0.2) || 
            (currentX < second_obstacle_xcoor && currentX > second_obstacle_xcoor - 0.2)) {
            move_forward(pwm * 0.5); // Move slower near obstacle columns
        } else {
            // Enes100.println("Path clear, moving forward");
            move_forward(pwm);
        }
        delay(100);
    }
    
    // Check if we've passed the second obstacle
    currentX = wifi_get_X();
    if (currentX > second_obstacle_xcoor) {
        is_obstacle_done = true;
        // Enes100.println("Obstacles cleared!");
    }
}

#endif