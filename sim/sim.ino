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
    // Enes100.println(Enes100.getTheta());
    
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
    
    // Enes100.println("Final angle: ");
    // Enes100.print(Enes100.getTheta());    
}

// This function moves either forward or backward until there is a obstacle.
void navigation(double pwm, double distFromObst) {
    Enes100.println("start navigation method");

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
    
    Enes100.println("stopped navigation method");
    }
    
// This function moves towards a specific X coordinate
void navigatingCoorX(double pwm, double finalX) {
    Enes100.println("start navigatingCoorX method");

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

    Enes100.println("stopped navigatingCoorX method");

}
    
// This function moves towards a specific Y coordinate
void navigatingCoorY(double pwm, double finalY) {
    Enes100.println("start navigatingCoorY method");

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
    
    Enes100.println("stopped navigatingCoorY method");
}

// navigate up one obstacle.
double navigatingAroundObstacle(double pwm) {
    Enes100.println("Starting avoiding obstacle method");
    double Y = Enes100.getY();
    Enes100.println("Y: ");
    Enes100.print(Y);
    
    double finalY = (Y + 0.5);
    
    Enes100.println("final Y: ");
    Enes100.print(finalY);


    navigatingCoorY(pwm, finalY);
    
    setAngle(0, 0.07, 50);
    ultrasonicReading = Tank.readDistanceSensor(1);
    
    Enes100.println("ultrasonicReading obstacles: ");  
    Enes100.print(ultrasonicReading);
    Enes100.println("stopped avoiding obstacle method");

    return ultrasonicReading;
}

// Function for getting through the arena.
void navigatingArena(double pwm) {
    Enes100.println("start navigating around arena method");    

    // Start at bottom of arena.
    navigatingCoorY(pwm, 0.5); 
    double ultrasonicReading = Tank.readDistanceSensor(1);

    double currX = Enes100.getX();
    
    // Move forward first
    navigation(pwm, 0.2);
    currX = Enes100.getX();
        
    // When stopped check if its before X = 1.9. 
    // If no, avoid obstacles (until theres no obstacle in front).
    if (currX < 1.9) {
        while ((ultrasonicReading <= 0.2) && (ultrasonicReading != -1)) {
            ultrasonicReading = navigatingAroundObstacle(pwm);
        }
        
        // Move to Y = 2.6.
        navigatingCoorY(pwm, 2.6); 
    }
        
    // Check its after X = 2.6.
    // If no, avoid obstacles and move to X = 2.6.
    currX = Enes100.getX();
    if (currX < 2.6) {
        // reset starting point to Y = 0.5.
        navigatingCoorY(pwm, 0.5); 
        while ((ultrasonicReading <= 0.2) && (ultrasonicReading != -1)) {
            ultrasonicReading = navigatingAroundObstacle(pwm);
        }
        navigatingCoorY(pwm, 2.6); 
    }
    
    Enes100.println("stopped navigating around obstacles method");    
}

// Function for going under the limbo
void limbo() {
    // Call naviagetToCoor to Y = 1.3
    navigatingCoorY(100, 1.4);
    // call naviagetToCoor to X = 3.7
    navigation(0.2, 100);
}

// code for navigation with obstacles
void setup() {
    Enes100.begin("Simulator", CRASH_SITE, 3, 8, 9);
    Tank.begin();
    Enes100.println("Start.");    
    
    // setAngle(0, 0.07, 50);
    // Enes100.println("Angle set to 0.");   
    
    navigatingArena(200);
    Enes100.println("Finished avoiding obstacles.");    
    // limbo();
    Enes100.println("Done.");    

}

void loop() {
    // delay(1000);
    // Enes100.print("Ultrasonic Reading: ");
    // Enes100.println(Tank.readDistanceSensor(1));
    // Enes100.print("X = "); Enes100.println(Enes100.getX());
    // Enes100.print("Y = "); Enes100.println(Enes100.getY());
    // Enes100.print("Theta = "); Enes100.println(Enes100.getTheta());
}