#include "sensors.h"
#include "Enes100.h"
#include "nav.h"

// Function for going under the limbo
void limbo(double pwm) {
    // Enes100.println("Navigating past limbo");
    navigatingCoorY(pwm, limbo_start_coor);
    navigation(pwm, 0.2);
    
    // Enes100.println("Limbo cleared!");
    is_limbo_done = true;
}