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

bool isPastSecondObstacle();
void handleFirstObstacleColumn(double pwm);
void handleSecondObstacleColumn(double pwm);
void handleGeneralObstacle(double pwm);
void moveForwardWithSpeedAdjustment(double pwm);
bool isNearObstacleColumn(double currentX);

void obstacle_setup() {
  // Always start at bottom obstacles
  navigatingCoorY(250, bottom_obstacle);
}

// Main function that orchestrates obstacle navigation
void navigatingAroundObstacle(double pwm) {
    // Enes100.println("Navigating around obstacles");
    double currentX = wifi_get_X();
    
    // Check if we've passed the second obstacle
    if (isPastSecondObstacle()) {
        return;
    }
    
    // Always make sure we're facing forward
    setAngle(0, 0.07, 50);
    
    // Specific handling based on position
    if (abs(currentX - first_obstacle_xcoor) < 0.1) {
        handleFirstObstacleColumn(pwm);
    } 
    else if (abs(currentX - second_obstacle_xcoor) < 0.1) {
        handleSecondObstacleColumn(pwm);
    }
    else {
        // Get current ultrasonic reading
        double ultrasonicReading = ultra_get_distance();
        
        if (ultrasonicReading != -1 && ultrasonicReading < 20) {
            handleGeneralObstacle(pwm);
        } 
        else {
            moveForwardWithSpeedAdjustment(pwm);
        }
    }
    
    // Check if we've passed the second obstacle
    if (isPastSecondObstacle()) {
        is_obstacle_done = true;
        // Enes100.println("Obstacles cleared!");
    }
}

// Check if the OSV has passed the second obstacle
bool isPastSecondObstacle() {
    double currentX = wifi_get_X();
    if (currentX > second_obstacle_xcoor) {
        is_obstacle_done = true;
        // Enes100.println("Obstacles cleared");
        return true;
    }
    return false;
}

// Logic for handling first obstacle column
void handleFirstObstacleColumn(double pwm) {
    // We're at the first obstacle column - force a stop and check
    stop_motors(); 
    delay(100);  
    
    // Make sure we're facing forward before checking
    setAngle(0, 0.07, 50);
    double ultrasonicReading = ultra_get_distance();
    double currentY = wifi_get_Y();

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

// Logic for handling second obstacle column
void handleSecondObstacleColumn(double pwm) {
    // We're at the second obstacle column - force a stop and check
    stop_motors(); 
    delay(100);  
    
    // Make sure we're facing forward before checking
    setAngle(0, 0.07, 50);
    double ultrasonicReading = ultra_get_distance();
    double currentY = wifi_get_Y();
    
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

// Logic for handling obstacles during general movement
void handleGeneralObstacle(double pwm) {
    stop_motors(); 
    double currentY = wifi_get_Y();
    
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

// Logic for moving forward with speed adjustment near obstacle columns
void moveForwardWithSpeedAdjustment(double pwm) {
    double currentX = wifi_get_X();
    
    // Slow down when approaching known obstacle columns
    if (isNearObstacleColumn(currentX)) {
        move_forward(pwm * 0.5); // Move slower near obstacle columns
    } else {
        // Enes100.println("Path clear, moving forward");
        move_forward(pwm);
    }
    delay(100);
}

// Helper to determine if we're approaching an obstacle column
bool isNearObstacleColumn(double currentX) {
    return (currentX < first_obstacle_xcoor && currentX > first_obstacle_xcoor - 0.2) || 
           (currentX < second_obstacle_xcoor && currentX > second_obstacle_xcoor - 0.2);
}