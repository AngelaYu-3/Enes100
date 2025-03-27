#include "Arduino.h"
#include "Enes100.h"
#include "Tank.h"

// this function calculates the most efficient direction counter or clockwise for the
// OTV to move in to turn to a set angle
double calculateEfficientAngle(int currAngle, int targetAngle) {
    currAngle = currAngle % 2*PI;
    if (currAngle < 0) currAngle += 2*PI;
    
    targetAngle = targetAngle % 2*PI;
    if (targetAngle < 0) targetAngle += 2*PI;
    
    double cDistance = (targetAngle - currAngle + 2*PI) % 2*PI;
    double ccDistance = (currAngle - targetAngle + 2*PI) % 2*PI;
    
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
        Enes100.print(currAngle - targetAngle);
        if (currAngle - targetAngle > 0) {
            // Enes100.print("print statement 1: ");
            Tank.setLeftMotorPWM(pwm);
            Tank.setRightMotorPWM(-pwm);
        } else if (currAngle - target < 0) {
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

// code for navigation without obstacles
void setup() {
    Enes100.begin("Simulator", CRASH_SITE, 3, 8, 9);
    Tank.begin();
    
    // Enes100.println("Starting driving");
    setAngle(-2.8, 0.08, 50);
    // Tank.setLeftMotorPWM(255);
    //Tank.setRightMotorPWM(255);
}

void loop() {
    // delay(1000);
    // Enes100.print("Ultrasonic Reading: ");
    // Enes100.println(Tank.readDistanceSensor(1));
    // Enes100.print("X = "); Enes100.println(Enes100.getX());
    // Enes100.print("Y = "); Enes100.println(Enes100.getY());
    // Enes100.print("Theta = "); Enes100.println(Enes100.getTheta());
}