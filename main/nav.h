#include "sensors.h"
#include "movement.h"
#include "Enes100.h"

const double first_obstacle_xcoor = 1.7;
const double second_obstacle_xcoor = 2.5;
const double bottom_obstacle = 0.5;
const double middle_obstacle = 1.0;
const double top_obstacle = 1.5;
const double limbo_start_coor = 1.5;

// This function moves either forward or backward until there is a obstacle.
void navigation(double pwm, double distFromObst) {
    double ultrasonicReading = sensor.ultra_get_distance();
        
        // While there is no obstacles, tank moves forward
        while (ultrasonicReading == -1 || ultrasonicReading > distFromObst) {
            movement.move_forward(pwm);
            ultrasonicReading = sensor.ultra_get_distance();
        } 
        
        // There is obstacles, so stop moving.
        movement.stop_motors();  

}

// This function moves towards a specific X coordinate
void navigatingCoorX(double pwm, double finalX) {
    double X = sensors.wifi_get_X();
    double threshold = 0.08;
    // backward can only be either 1 (true) or -1 (false).
    int backward = -1;
       
    if (X < finalX) {
        movement.setAngle(0, 0.09, 50);
    } else {
        movement.setAngle(-PI, 0.09, 50);
        backward = 1;
    }
    
    // If X is not near coor
    while (!(X < (finalX + threshold) && X > (finalX - threshold))) {
        movement.move_forward(pwm);
        
        X = sensors.wifi_get_X();
    }

    // Stop moving.
    movement.stop_motors();
    
    // Face defualt theta.
    if (backward == 1) {
        movement.setAngle(0, 0.08, 50);
    }
}

// This function moves towards a specific Y coordinate
void navigatingCoorY(double pwm, double finalY) {
    double Y = sensors.wifi_get_Y();
    double threshold = 0.09;
       
    // Depending on if robot is below or above, turn left or right from default theta.
    if (Y < finalY) {
        movement.setAngle(PI/2, 0.09, 50);
    } else {
        movement.setAngle(-PI/2, 0.09, 50);
    }
    
    // If Y is not in targetCoorX. 
    while (!(Y < (finalY + threshold) && Y > (finalY - threshold))) {
        movement.move_forward(pwm);
        
        Y = sensors.wifi_get_Y();
    }

    // Stop moving, face default theta.
    movement.stop_motors(); 
    movement.setAngle(0, 0.09, 50);
}

// This function contains the logic behind the avoiding obstacles mission. 
void navigatingAroundObstacle(double pwm) {
    // Enes100.println("Navigating around obstacles");
    double currentX = sensors.wifi_get_X();
    double currentY = sensors.wifi_get_Y();
    double ultrasonicReading;
    
    // Check if we've passed the second obstacle
    if (currentX > second_obstacle_xcoor) {
        is_obstacle_done = true;
        // Enes100.println("Obstacles cleared");
        return;
    }
    
    // Always make sure we're facing forward
    movement.setAngle(0, 0.07, 50);
    
    // Get current ultrasonic reading
    ultrasonicReading = sensor.ultra_get_distance();
    
    // Specific handling for first obstacle column
    if (abs(currentX - first_obstacle_xcoor) < 0.1) {
        // We're at the first obstacle column - force a stop and check
        movement.stop_motors(); 
        delay(100);  
        
        // Make sure we're facing forward before checking
        movement.setAngle(0, 0.07, 50);
        ultrasonicReading = sensor.ultra_get_distance();

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
            movement.move_forward(pwm);
            delay(150);
        }
    } 
    // Specific handling for second obstacle column
    else if (abs(currentX - second_obstacle_xcoor) < 0.1) {
        // We're at the second obstacle column - force a stop and check
        movement.stop_motors(); 
        delay(100);  
        
        // Make sure we're facing forward before checking
        movement.setAngle(0, 0.07, 50);
        ultrasonicReading = Tank.readDistanceSensor(1);
        
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
            movement.move_forward(pwm);
            delay(100);
        }
    }
    // Standard obstacle detection during general movement
    else if (ultrasonicReading != -1 && ultrasonicReading < 20) {
        movement.stop_motors(); 
        
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
            movement.move_forward(pwm * 0.5); // Move slower near obstacle columns
        } else {
            // Enes100.println("Path clear, moving forward");
            movement.move_forward(pwm);
        }
        delay(100);
    }
    
    // Check if we've passed the second obstacle
    currentX = sensors.wifi_get_X();
    if (currentX > second_obstacle_xcoor) {
        is_obstacle_done = true;
        // Enes100.println("Obstacles cleared!");
    }
}

// Function for going under the limbo
void limbo(double pwm) {
    // Enes100.println("Navigating past limbo");
    navigatingCoorY(pwm, limbo_start_coor);
    navigation(pwm, 0.2);
    
    // Enes100.println("Limbo cleared!");
    is_limbo_done = true;
}

