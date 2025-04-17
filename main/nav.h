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
        move_forward(pwm);
        
        X = wifi_get_X();
    }

    // Stop moving.
    stop_motors();
    
    // Face defualt theta.
    if (backward == 1) {
        setAngle(0, 0.08, 50);
    }
}

// This function moves towards a specific Y coordinate
void navigatingCoorY(double pwm, double finalY) {
    double Y = wifi_get_Y();
    double threshold = 0.09;
       
    // Depending on if robot is below or above, turn left or right from default theta.
    if (Y < finalY) {
        setAngle(PI/2, 0.09, 50);
    } else {
        setAngle(-PI/2, 0.09, 50);
    }
    
    // If Y is not in targetCoorX. 
    while (!(Y < (finalY + threshold) && Y > (finalY - threshold))) {
        move_forward(pwm);
        
        Y = wifi_get_Y();
    }

    // Stop moving, face default theta.
    stop_motors(); 
    setAngle(0, 0.09, 50);
}
