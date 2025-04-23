#ifndef NAV_H
#define NAV_H

#include "sensors.h"
#include "movement.h"
#include "Enes100.h"

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
    Enes100.println(Enes100.getTheta());
    
    while (!(currAngle < (targetAngle + threshold) && currAngle > (targetAngle - threshold))) {
        //Enes100.print(currAngle - targetAngle);
        if (currAngle - targetAngle > 0) {
            // Enes100.print("print statement 1: ");
            turn_right(100);
        } else if (currAngle - targetAngle < 0) {
            // Enes100.println("print statement 2");
            turn_left(100);
        } 
        currAngle = Enes100.getTheta();
    }
    stop_motors();  
    
    // Enes100.print("Final angle: ");
    Enes100.println(Enes100.getTheta());    
}

// This function moves either forward or backward until there is a obstacle.
void navigation(double pwm, double distFromObst) {
    double ultrasonicReading = ultra_get_distance();
        
        // While there is no obstacles, tank moves forward
        while (ultrasonicReading == -1 || ultrasonicReading > distFromObst) {
            move_forward(pwm);
            ultrasonicReading = ultra_get_distance();
        } 
        
        // There is obstacles, so stop moving.
        stop_motors();  

}

// This function moves towards a specific X coordinate
void navigatingCoorX(double pwm, double finalX) {
    double X = wifi_get_X();
    double threshold = 0.08;

    // Make sure OTV is facing default theta.
    setAngle(0, 0.09, 50);
       
    // If we need to move backward, negate pwm so wheel move backward.
    if (X > finalX) {
        pwm = -pwm;
    } 

    // If X is not near coor.
    while (!(X < (finalX + threshold) && X > (finalX - threshold))) {
        move_forward(pwm);
        
        X = wifi_get_X();
    }

    // Stop moving.
    stop_motors();
}

// This function moves towards a specific Y coordinate
void navigatingCoorY(double pwm, double finalY) {
    double Y = wifi_get_Y();
    double threshold = 0.09;
    
    // Make sure OTV is facing default theta.
    setAngle(0, 0.09, 50);

    // If Y is not in targetCoorY. 
    while (!(Y < (finalY + threshold) && Y > (finalY - threshold))) {
        // Depending on if robot is below or above, shift left or right from default theta.
        if (Y < finalY) {
            shift_left(pwm);
        } else {
            shift_right(pwm);
        }
        
        Y = wifi_get_Y();
    }

    // Stop moving, face default theta.
    stop_motors(); 
}

#endif