#include "Arduino.h"
#include "Enes100.h"
#include "Tank.h"

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
            Tank.setLeftMotorPWM(pwm);
            Tank.setRightMotorPWM(-pwm);
        } else if (currAngle - targetAngle < 0) {
            // Enes100.println("print statement 2");
            Tank.setLeftMotorPWM(-pwm);
            Tank.setRightMotorPWM(pwm);
        } 
        currAngle = Enes100.getTheta();
    }
    Tank.setLeftMotorPWM(0);
    Tank.setRightMotorPWM(0);  
    
    // Enes100.print("Final angle: ");
    Enes100.println(Enes100.getTheta());    
}

void navigatingForward(double dist, double pwm) {
    double ultrasonicReading = Tank.readDistanceSensor(1);
    
    // Moves forward until it reaches the end of the arena 
    while (Enes100.getX < 3.7) { 
        
        // While there is no obstacles, tank moves forward
        while (ultrasonicReading == -1 || ultrasonicReading > dist) {
            Tank.setLeftMotorPWM(pwm);
            Tank.setRightMotorPWM(pwm);
            ultrasonicReading = Tank.readDistanceSensor(1);
        } 
        
        // There is obstacles, so moves either left or right.
        avoidingObstacles(pwm);
    }  
}

void avoidingObstacles(double pwm) {
    // Tank stops moving.
    Tank.setLeftMotorPWM(0);
    Tank.setRightMotorPWM(0); 
    double Y = Enes100.getY();
    
    // If the vehicle is near the bottom of the arena (Y < 0.65)
    if (Y < 0.65) {
        // Turn left (toward the center)
        setAngle(-PI/2, 0.09, 50);
        
        // Move until Y reaches approximately 1.0 (center zone)
        while (Y < 1.0) {
            Tank.setLeftMotorPWM(pwm);
            Tank.setRightMotorPWM(pwm);
            Y = Enes100.getY();
        }
        
        // Check if there's still an obstacle
        if (Tank.readDistanceSensor(1) > 0.2) {
            // No obstacle, continue forward
            setAngle(0, 0.09, 50);
        } else {
            // Still obstacle, continue turning left
            setAngle(-PI/2, 0.09, 50);
        }
    } 
    // If the vehicle is near the top of the arena (Y > 1.35)
    else if (Y > 1.35) {
        // Turn right (toward the center)
        setAngle(PI/2, 0.09, 50);
        
        // Move until Y reaches approximately 1.0 (center zone)
        while (Y > 1.0) {
            Tank.setLeftMotorPWM(pwm);
            Tank.setRightMotorPWM(pwm);
            Y = Enes100.getY();
        }
        
        // Check if there's still an obstacle
        if (Tank.readDistanceSensor(1) > 0.2) {
            // No obstacle, continue forward
            setAngle(0, 0.09, 50);
        } else {
            // Still obstacle, continue turning right
            setAngle(PI/2, 0.09, 50);
        }
    }
    // If the vehicle is in the middle zone (0.65 <= Y <= 1.35)
    else {
        // Choose to go left arbitrarily
        setAngle(-PI/2, 0.09, 50);
        
        // Move for a bit to get around the obstacle
        for (int i = 0; i < 100; i++) {
            Tank.setLeftMotorPWM(pwm);
            Tank.setRightMotorPWM(pwm);
            delay(10); // Move for 1 second total
        }
        
        // Check if there's still an obstacle
        if (Tank.readDistanceSensor(1) > 0.2) {
            // No obstacle, continue forward
            setAngle(0, 0.09, 50);
        }
    }
    
    // Reset vehicle to forward orientation
    Tank.setLeftMotorPWM(0);
    Tank.setRightMotorPWM(0);
}

// code for navigation with obstacles
void setup() {
    Enes100.begin("Simulator", CRASH_SITE, 3, 8, 9);
    Tank.begin();
    
    // Enes100.println("Starting driving");
    setAngle(0, 0.09, 50);
    navigatingForward(0.2, 75);
}

void loop() {
    // delay(1000);
    // Enes100.print("Ultrasonic Reading: ");
    // Enes100.println(Tank.readDistanceSensor(1));
    // Enes100.print("X = "); Enes100.println(Enes100.getX());
    // Enes100.print("Y = "); Enes100.println(Enes100.getY());
    // Enes100.print("Theta = "); Enes100.println(Enes100.getTheta());
}