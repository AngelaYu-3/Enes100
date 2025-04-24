#ifndef MOVEMENT_H
#define MOVEMENT_H

#include "Enes100.h"
#include "sensors.h"
// looking at robot from back
// motor A connections H-Bridge 1 (back-left)
const int enA = 10;
const int in1 = 9;
const int in2 = 28;

// motor B connections H-Bridge 1 (back-right)
const int enB = 8;
const int in3 = 30;
const int in4 = 32;

// motor C connections H-Bridge 2 (front-right)
const int enC = 7;
const int in5 = 29;
const int in6 = 31;

// motor D connections H-Bridge 2 (front-left)
const int enD = 5;
const int in7 = 6;
const int in8 = 33;

void motor_setup() {
  // set all motor control pins to outputs
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(enC, OUTPUT);
  pinMode(enD, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  pinMode(in5, OUTPUT);
  pinMode(in6, OUTPUT);
  pinMode(in7, OUTPUT);
  pinMode(in8, OUTPUT);

  // turn off motors - initial state
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
  digitalWrite(in5, LOW);
  digitalWrite(in6, LOW);
  digitalWrite(in7, LOW);
  digitalWrite(in8, LOW);
}

void control_motor_A(int speed, bool is_forward) {
  analogWrite(enA, speed);

  if (is_forward) {
    digitalWrite(in1, LOW); // low
    digitalWrite(in2, HIGH);
  } else {
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW); 
  }
}

void control_motor_B(int speed, bool is_forward) {
  analogWrite(enB, speed);

  if (is_forward) {
    digitalWrite(in3, LOW); // low
    digitalWrite(in4, HIGH);
  } else {
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW); 
  }
}

void control_motor_C(int speed, bool is_forward) {
  analogWrite(enC, speed);

  if (is_forward) {
    digitalWrite(in5, HIGH);
    digitalWrite(in6, LOW); // low
  } else {
    digitalWrite(in5, LOW);
    digitalWrite(in6, HIGH); 
  }
}

void control_motor_D(int speed, bool is_forward) {
  analogWrite(enD, speed);

  if (is_forward) {
    digitalWrite(in7, HIGH);
    digitalWrite(in8, LOW); // low
  } else {
    digitalWrite(in7, LOW);
    digitalWrite(in8, HIGH); 
  }
}

void stop_motors() {
	digitalWrite(in1, LOW);
	digitalWrite(in2, LOW);
	digitalWrite(in3, LOW);
	digitalWrite(in4, LOW);
	digitalWrite(in5, LOW);
	digitalWrite(in6, LOW);
	digitalWrite(in7, LOW);
	digitalWrite(in8, LOW);
}

void move_forward(int speed) {
  control_motor_A(speed, true);
  control_motor_B(speed, true);
  control_motor_C(speed, true);
  control_motor_D(speed, true);
}

void move_backward(int speed) {
  control_motor_A(speed, false);
  control_motor_B(speed, false);
  control_motor_C(speed, false);
  control_motor_D(speed, false);
}

void turn_left(int speed) {
  control_motor_A(speed, false);
  control_motor_C(speed, true);
  control_motor_B(speed, true);
  control_motor_D(speed, false);
}

void turn_right(int speed) {
  control_motor_A(speed, true);
  control_motor_C(speed, false);
  control_motor_B(speed, false);
  control_motor_D(speed, true);
}

void shift_left(int speed) {
  control_motor_A(speed, true);
  control_motor_B(speed, false);
  control_motor_C(speed, true);
  control_motor_D(speed, false);
}

void shift_right(int speed) {
  control_motor_A(speed, false);
  control_motor_B(speed, true);
  control_motor_C(speed, false);
  control_motor_D(speed, true); 
}

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

// uses ultrasonic sensor to move to a set distance (cm)
void move_to_dist(int dist, int threshold) {
  int curr_dist = ultra_get_distance();
  int low_thresh = dist - threshold;
  int upper_thresh = dist + threshold;

  while (curr_dist < low_thresh || curr_dist > upper_thresh) {
    if (curr_dist < low_thresh) {
      move_backward(100);
    } else if (curr_dist > upper_thresh) {
      move_forward(100);
    }
    curr_dist = ultra_get_distance();
  }

  stop_motors();
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

// This function moves towards a specific X coordinate
void navigatingCoorX(double pwm, double finalX) {
    double X = wifi_get_X();
    double threshold = 0.08;

    // Make sure OTV is facing default theta.
    setAngle(PI/2, 0.09, 50);
       
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
    setAngle(-PI/2, 0.5, 50);

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