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

// This function moves forward until there is a obstacle.
void navigatingForward(double dist, double pwm) {
    double ultrasonicReading = Tank.readDistanceSensor(1);
        
        // While there is no obstacles, tank moves forward
        while (ultrasonicReading == -1 || ultrasonicReading > dist) {
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
    double threshold = 0.09;
       
    if (X < finalX) {
        setAngle(0, 0.09, 50);
    } else {
        setAngle(-PI, 0.09, 50);
    }
    
    // If X is not near coor
    while (!(X < (finalX + threshold) && X > (finalX - threshold))) {
        Tank.setLeftMotorPWM(pwm);
        Tank.setRightMotorPWM(pwm); 
        
        X = Enes100.getX();
    }

    // Stop moving, face default theta.
    Tank.setLeftMotorPWM(0);
    Tank.setRightMotorPWM(0); 
    setAngle(0, 0.09, 50);
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

// int left can only be either 1 (true) or -1 (false).
void navigatingAroundObstacle(double pwm, int left) {
    if (left == 1) {
        navigatingCoorY(100, (Y + 0.5));
    } else {
        navigatingCoorY(100, (Y - 0.5));
    }
}

// Function for getting through the arena.
void navigatingArena() {
    navigatingCoorY(150, 0.5) {
    while (X <= 1.3) {
        navigatingForward(0.2, 100);
        double X = Enes100.getX();

        navigatingCoorY(100, (Y + 0.5));
        double X = Enes100.getX();
    }
    
    navigatingCoorY(150, 0.5) {
    while (X <= 2.85) {
        navigatingForward(0.2, 100);
        double X = Enes100.getX();

        navigatingCoorY(100, (Y + 0.5));
        double X = Enes100.getX();
    }
}

// Function for going under the limbo
void limbo() {
    // Call naviagetToCoor to Y = 1.3
    navigatingCoorY(100, 1.4);
    // call naviagetToCoor to X = 3.7
    navigatingForward(0.2, 100);
}

// code for navigation with obstacles
void setup() {
    Enes100.begin("Simulator", CRASH_SITE, 3, 8, 9);
    Tank.begin();
    
    // Start facing towards the right.
    setAngle(0, 0.09, 50);
    // Start at the bottom of the map (y=0.5).
    navigatingArena();
    // navigatingCoorX(150, 0.5) {
    // limbo();
}

void loop() {
    // delay(1000);
    // Enes100.print("Ultrasonic Reading: ");
    // Enes100.println(Tank.readDistanceSensor(1));
    // Enes100.print("X = "); Enes100.println(Enes100.getX());
    // Enes100.print("Y = "); Enes100.println(Enes100.getY());
    // Enes100.print("Theta = "); Enes100.println(Enes100.getTheta());
}