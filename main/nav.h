#ifndef NAV_H
#define NAV_H

#include "sensors.h"
#include "movement.h"
#include "Enes100.h"

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
#endif