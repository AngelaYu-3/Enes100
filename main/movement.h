#ifndef MOVEMENT_H
#define MOVEMENT_H

#include "Enes100.h"
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

#endif