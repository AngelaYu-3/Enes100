#include "Arduino.h"
#include "Enes100.h"
#include "Tank.h"

const double first_obstacle_xcoor = 1.7;
const double second_obstacle_xcoor = 2.5;
const double bottom_obstacle = 0.5;
const double middle_obstacle = 1.0;
const double top_obstacle = 1.5;
const double limbo_start_coor = 1.5;


bool is_mission_done = true;
bool is_obstacle_done = false;
bool is_limbo_done = false;


// this function calculates the most efficient direction counter or clockwise for the
// OTV to move in to turn to a set angle
double calculateEfficientAngle(int currAngle, int targetAngle) {
    // currAngle = currAngle % 2*PI;
    if (currAngle < 0) currAngle += 2*PI;
    
    // targetAngle = targetAngle % 2*PI;
    if (targetAngle < 0) targetAngle += 2*PI;
    
    double cDistance = (targetAngle - currAngle + 2*PI);
    double ccDistance = (currAngle - targetAngle + 2*PI);
    
    if (cDistance <= ccDistance) {
        return cDistance;
    } else {
        return -ccDistance;
    }
}

// this function makes the OTV turn to a certain angle 
void setAngle(double targetAngle, double threshold, double pwm) {
    double currAngle = Enes100.getTheta();
    double rotAmount = calculateEfficientAngle(currAngle, targetAngle);

    while (!(currAngle < (targetAngle + threshold) && currAngle > (targetAngle - threshold))) {
        if (currAngle - targetAngle > 0) {
            Tank.setLeftMotorPWM(pwm);
            Tank.setRightMotorPWM(-pwm);
        } else if (currAngle - targetAngle < 0) {
            Tank.setLeftMotorPWM(-pwm);
            Tank.setRightMotorPWM(pwm);
        } 
        currAngle = Enes100.getTheta();
    }
    Tank.setLeftMotorPWM(0);
    Tank.setRightMotorPWM(0);  
}

// This function moves either forward or backward until there is a obstacle.
void navigation(double pwm, double distFromObst) {
    double ultrasonicReading = Tank.readDistanceSensor(1);
        
        // While there is no obstacles, tank moves forward
        while (ultrasonicReading == -1 || ultrasonicReading > distFromObst) {
            Tank.setLeftMotorPWM(pwm);
            Tank.setRightMotorPWM(pwm);
            ultrasonicReading = Tank.readDistanceSensor(1);
        } 
        
        // There is obstacles, so stop moving.
        Tank.setLeftMotorPWM(0);
        Tank.setRightMotorPWM(0); 
}
    
// This function moves towards a specific X coordinate
void navigatingCoorX(double pwm, double finalX) {
    double X = Enes100.getX();
    double threshold = 0.08;
    // backward can only be either 1 (true) or -1 (false).
    int backward = -1;
       
    if (X < finalX) {
        setAngle(0, 0.09, 50);
    } else {
        setAngle(-PI, 0.09, 50);
        backward = 1;
    }
    
    // If X is not near coor
    while (!(X < (finalX + threshold) && X > (finalX - threshold))) {
        Tank.setLeftMotorPWM(pwm);
        Tank.setRightMotorPWM(pwm); 
        
        X = Enes100.getX();
    }

    // Stop moving.
    Tank.setLeftMotorPWM(0);
    Tank.setRightMotorPWM(0);
    
    // Face defualt theta.
    if (backward == 1) {
        setAngle(0, 0.08, 50);
    }
}
    
// This function moves towards a specific Y coordinate
void navigatingCoorY(double pwm, double finalY) {
    double Y = Enes100.getY();
    double threshold = 0.09;
       
    // Depending on if robot is below or above, turn left or right from default theta.
    if (Y < finalY) {
        setAngle(PI/2, 0.09, 50);
    } else {
        setAngle(-PI/2, 0.09, 50);
    }
    
    // If Y is not in targetCoorX. 
    while (!(Y < (finalY + threshold) && Y > (finalY - threshold))) {
        Tank.setLeftMotorPWM(pwm);
        Tank.setRightMotorPWM(pwm); 
        
        Y = Enes100.getY();
    }

    // Stop moving, face default theta.
    Tank.setLeftMotorPWM(0);
    Tank.setRightMotorPWM(0); 
    setAngle(0, 0.09, 50);
}

void navigatingAroundObstacle(double pwm) {
    Enes100.println("Navigating around obstacles");
    double currentX = Enes100.getX();
    double currentY = Enes100.getY();
    double ultrasonicReading;
    
    // Check if we've passed the second obstacle
    if (currentX > second_obstacle_xcoor) {
        is_obstacle_done = true;
        Enes100.println("Obstacles cleared!");
        return;
    }
    
    // Always make sure we're facing forward
    setAngle(0, 0.07, 50);
    
    // Get current ultrasonic reading
    ultrasonicReading = Tank.readDistanceSensor(1);
    
    // Specific handling for first obstacle column
    if (abs(currentX - first_obstacle_xcoor) < 0.1) {
        // We're at the first obstacle column - force a stop and check
        Tank.setLeftMotorPWM(0);
        Tank.setRightMotorPWM(0);
        delay(100);  
        
        // Make sure we're facing forward before checking
        setAngle(0, 0.07, 50);
        ultrasonicReading = Tank.readDistanceSensor(1);
        
        // Check if there's an obstacle
        if (ultrasonicReading != -1 && ultrasonicReading < 20) {
            // If not at the top, move up by 0.5
            if (currentY < top_obstacle) {
                Enes100.println("Obstacle at first column, moving up");
                navigatingCoorY(pwm, currentY + 0.5);
            } else {
                Enes100.println("At top of first column, moving down");
                navigatingCoorY(pwm, currentY - 0.5);
            }
        } else {
            // No obstacle at this Y position, continue forward
            Enes100.println("No obstacle detected at first column, continuing");
            Tank.setLeftMotorPWM(pwm);
            Tank.setRightMotorPWM(pwm);
            delay(150);
        }
    } 
    // Specific handling for second obstacle column
    else if (abs(currentX - second_obstacle_xcoor) < 0.1) {
        // We're at the second obstacle column - force a stop and check
        Tank.setLeftMotorPWM(0);
        Tank.setRightMotorPWM(0);
        delay(100);  
        
        // Make sure we're facing forward before checking
        setAngle(0, 0.07, 50);
        ultrasonicReading = Tank.readDistanceSensor(1);
        
        // Check if there's an obstacle
        if (ultrasonicReading != -1 && ultrasonicReading < 20) {
            // If not at the top, move up by 0.5
            if (currentY < top_obstacle) {
                Enes100.println("Obstacle at second column, moving up");
                navigatingCoorY(pwm, currentY + 0.5);
            } else {
                Enes100.println("At top of second column, moving down");
                navigatingCoorY(pwm, currentY - 0.5);
            }
        } else {
            // No obstacle at this Y position, continue forward
            Enes100.println("No obstacle detected at second column, continuing");
            Tank.setLeftMotorPWM(pwm);
            Tank.setRightMotorPWM(pwm);
            delay(100);
        }
    }
    // Standard obstacle detection during general movement
    else if (ultrasonicReading != -1 && ultrasonicReading < 20) {
        Tank.setLeftMotorPWM(0);
        Tank.setRightMotorPWM(0);
        
        // If not at the top, move up by 0.5
        if (currentY < top_obstacle) {
            Enes100.println("Obstacle detected, moving up by 0.5");
            navigatingCoorY(pwm, currentY + 0.5);
        } 
        // If at the top, start moving down by 0.5
        else {
            Enes100.println("At top, moving down by 0.5");
            navigatingCoorY(pwm, currentY - 0.5);
        }
    } 
    // No obstacle detected, continue moving forward with speed adjustment
    else {
        // Slow down when approaching known obstacle columns
        if ((currentX < first_obstacle_xcoor && currentX > first_obstacle_xcoor - 0.2) || 
            (currentX < second_obstacle_xcoor && currentX > second_obstacle_xcoor - 0.2)) {
            Tank.setLeftMotorPWM(pwm * 0.5);  // Move slower near obstacle columns
            Tank.setRightMotorPWM(pwm * 0.5);
        } else {
            Enes100.println("Path clear, moving forward");
            Tank.setLeftMotorPWM(pwm);
            Tank.setRightMotorPWM(pwm);
        }
        delay(100);
    }
    
    // Check if we've passed the second obstacle
    currentX = Enes100.getX();
    if (currentX > second_obstacle_xcoor) {
        is_obstacle_done = true;
        Enes100.println("Obstacles cleared!");
    }
}

// Function for going under the limbo
void limbo(double pwm) {
    Enes100.println("Navigating past limbo");
    navigatingCoorY(pwm, limbo_start_coor);
    navigation(pwm, 0.2);
    
    Enes100.println("Limbo cleared!");
    is_limbo_done = true;
}

// code for navigation with obstacles
void setup() {
    Enes100.begin("Simulator", CRASH_SITE, 3, 8, 9);
    Tank.begin();

    // // Always start facing theta = 0.
    // setAngle(0, 0.07, 50);
    // Always start at bottom obstacles
    navigatingCoorY(250, bottom_obstacle);
}

void loop() {
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
        navigatingAroundObstacle(250);
    } else if (!is_limbo_done) {   
        limbo(250);
    } else {
        Tank.setLeftMotorPWM(0);
        Tank.setRightMotorPWM(0); 
    }
    
    delay(100); 
}