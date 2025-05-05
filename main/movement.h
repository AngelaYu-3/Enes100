#ifndef MOVEMENT_H
#define MOVEMENT_H

#include "Enes100.h"
#include "sensors.h"

/*
 * Setup and functions for motors.
 */

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
  control_motor_C(speed * 1.25, true);
  control_motor_D(speed, true);
}

void move_backward(int speed) {
  control_motor_A(speed, false);
  control_motor_B(speed, false);
  control_motor_C(speed * 1.25, false);
  control_motor_D(speed, false);
}

void turn_left(int speed) {
  control_motor_A(speed * 1.25, false);
  control_motor_C(speed, true);
  control_motor_B(speed * 1.25, true);
  control_motor_D(speed, false);
}

void turn_right(int speed) {
  control_motor_A(speed * 1.25, true);
  control_motor_C(speed, false);
  control_motor_B(speed * 1.25, false);
  control_motor_D(speed, true);
}

void shift_left(int speed) {
  control_motor_A(speed, true);
  control_motor_B(speed, false * 1.25);
  control_motor_C(speed, true * 0.75);
  control_motor_D(speed, false * 1.25);
}

void shift_right(int speed) {
  control_motor_A(speed, false);
  control_motor_B(speed, true * 1.25);
  control_motor_C(speed, false * 0.75);
  control_motor_D(speed, true * 1.25); 
}

// use ultrasonic sensor to move to a set distance (cm)
void move_to_dist_for(double dist, double threshold, double speed) {
  int curr_dist = ultra_get_distance();
  int upper_thresh = dist + threshold;

  while (curr_dist > upper_thresh) {
    move_forward(speed);
    curr_dist = ultra_get_distance();
    Enes100.println("moving to distance");
  }

  stop_motors();
}

void move_to_dist_back(double dist, double threshold, double speed) {
  int curr_dist = ultra_get_distance();
  int low_thresh = dist - threshold;

  while (curr_dist < low_thresh) {
    move_backward(speed);
    curr_dist = ultra_get_distance();
    Enes100.println("moving to distance");
  }

  stop_motors();
}

/*
 * sets OTV to defined target_angle
 */
void set_angle_simple(double target_angle, double thresh) {
  double current_angle = Enes100.getTheta();
  double low_thresh = target_angle - thresh;
  double upper_thresh = target_angle + thresh;

  while (current_angle < low_thresh || current_angle > upper_thresh) {
    if (target_angle - current_angle < 0) {
      turn_right(60);
    } else {
      turn_left(60);
    } 
    current_angle = Enes100.getTheta();
  }
  
  Enes100.println("setting angle!");
  stop_motors();

}

// This function moves towards a specific X coordinate
void nav_x(double pwm, double final_x, bool is_forward) {
    double x = Enes100.getX();
    double threshold = 0.1;

    // Make sure OTV is facing default theta.
    // set_angle(0, 0.09, 50);
       
    // If we need to move backward, negate pwm so wheel move backward.
    // if (X > finalX) {
    //     pwm = -pwm;
    // } 

    // If X is not near coor.
    while (x < (final_x - threshold) || x > (final_x + threshold)) {
      if (is_forward) {
        move_forward(pwm);
      } else {
        move_backward(pwm);
      }
      x = Enes100.getX();
    }

    // Stop moving.
    stop_motors();
}

// This function moves towards a specific Y coordinate
void nav_y(double pwm, double final_y) {
    double y = Enes100.getY();
    double threshold = 0.1;
    
    // Make sure OTV is facing default theta.
    // set_angle(0, 0.09, 50);

    // If Y is not in targetCoorY. 
    while (y < (final_y - threshold) || y > (final_y + threshold)) {
        // Depending on if robot is below or above, shift left or right from default theta.
        if (y < (final_y - threshold)) {
            shift_left(pwm);
        } else if (y > (final_y + threshold)){
            shift_right(pwm);
        }
        
        y = Enes100.getY();
    }

    // Stop moving, face default theta.
    stop_motors(); 
}


#endif