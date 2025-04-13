#include "Enes100.h"

// motor A connections H-Bridge 1 (left-front)
const int enA = 10;
const int in1 = 31;
const int in2 = 33;

// motor B connections H-Bridge 1 (right-front)
const int enB = 8;
const int in3 = 9;
const int in4 = 35;
// motor C connections H-Bridge 2 (left-back)
const int enC = 0;
const int in5 = 0;
const int in6 = 0;

// motor D connections H-Bridge 3 (right-back)
const int enD = 0;
const int in7 = 0;
const int in8 = 0;

void motor_setup() {
  // set all motor control pins to outputs
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);

  // turn off motors - initial state
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
}

void control_motor_A(int speed, bool is_forward) {
  analogWrite(enA, speed);

  if (is_forward) {
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
  } else {
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW); 
  }
}

void control_motor_B(int speed, bool is_forward) {
  analogWrite(enB, speed);

  if (is_forward) {
    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH);
  } else {
    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH); 
  }
}

void control_motor_C(int speed, bool is_forward) {
  analogWrite(enC, speed);

  if (is_forward) {
    digitalWrite(in5, LOW);
    digitalWrite(in6, HIGH);
  } else {
    digitalWrite(in5, LOW);
    digitalWrite(in6, HIGH); 
  }
}

void control_motor_D(int speed, bool is_forward) {
  analogWrite(enD, speed);

  if (is_forward) {
    digitalWrite(in7, LOW);
    digitalWrite(in8, HIGH);
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
  control_motor_A(speed, true);
  control_motor_C(speed, true);
  control_motor_B(speed, false);
  control_motor_D(speed, false);
}

void turn_right(int speed) {
  control_motor_A(speed, false);
  control_motor_C(speed, false);
  control_motor_B(speed, true);
  control_motor_D(speed, true);
}

void shift_left(int speed) {
  control_motor_A(speed, false);
  control_motor_B(speed, true);
  control_motor_C(speed, true);
  control_motor_D(speed, false);
}

void shift_right(int speed) {
  control_motor_A(speed, true);
  control_motor_B(speed, false);
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

