#include "sensors.h"
#include "movement.h"
#include "Enes100.h"

void setup() {
}

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
