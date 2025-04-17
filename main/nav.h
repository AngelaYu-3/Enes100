#include "sensors.h"
#include "movement.h"
#include "Enes100.h"

void setup() {
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
