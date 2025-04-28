#ifndef SENSORS_H
#define SENSORS_H

#include "Enes100.h"

// ultrasonic
#define trig_pin 25
#define echo_pin 23

void ultra_setup() {
  pinMode(trig_pin, OUTPUT);
  pinMode(echo_pin, INPUT);
  Serial.begin(9600);
}

// getting distance in cm
int ultra_get_distance() {
  long duration;
  long distance;

  digitalWrite(trig_pin, LOW);
  delayMicroseconds(2);

  digitalWrite(trig_pin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig_pin, LOW);

  duration = pulseIn(echo_pin, HIGH);
  // Serial.print("Duration: ");
  //Serial.println(duration);
  distance = (duration * 0.034) / 2;
  // Serial.print("Distance: ");
  // Serial.println(distance);
  return distance;
}


// color sensor
#define s0 4
#define s1 48
#define s2 3
#define s3 2
#define out 50

int red = 0;
int blue = 0;
int green = 0;

void color_setup() {
  pinMode(s0, OUTPUT);
  pinMode(s1, OUTPUT);
  pinMode(s2, OUTPUT);
  pinMode(s3, OUTPUT);
  pinMode(out, INPUT);
  Serial.begin(9600);

  digitalWrite(s0, HIGH);
  digitalWrite(s1, HIGH);
}

void get_colors() {
  digitalWrite(s2, LOW);
  digitalWrite(s3, LOW);
  red = pulseIn(out, digitalRead(out) == HIGH ? LOW : HIGH);
  Serial.print("red: ");
  Serial.println(red);
  delay(20);

  digitalWrite(s2, LOW);
  digitalWrite(s3, HIGH);
  blue = pulseIn(out, digitalRead(out) == HIGH ? LOW : HIGH);
  Serial.print("blue: ");
  Serial.println(blue);
  delay(20);

  digitalWrite(s2, HIGH);
  digitalWrite(s3, HIGH);
  green = pulseIn(out, digitalRead(out) == HIGH ? LOW : HIGH);
  Serial.print("green: ");
  Serial.println(green);
  delay(20);
}

bool is_red() { // grey: 60 150 115.   red:  38 180 180.       grey:       red: 10 25 20
  get_colors();
  Serial.print("isRed: ");
  if (blue >= 34 && green >= 24) {
        Serial.println("true");
        return true;
  } else {
    Serial.println("false");
    return false;
  }
}


// wifi sensor

float wifi_get_X() {
  Enes100.print("X: ");
  Enes100.println(Enes100.getX());
  //delay(1000);
  return Enes100.getX();
}

float wifi_get_Y() {
  Enes100.print("Y: ");
  Enes100.println(Enes100.getY());
  //delay(1000);
  return Enes100.getY();
}

float wifi_get_theta() {
  Enes100.print("Theta: ");
  Enes100.println(Enes100.getTheta());
  //delay(1000);
  return Enes100.getTheta();
}

// length is in mm
void wifi_transmit_length(float length) {
  Enes100.mission(LENGTH, length);
}

// heigght is in mm
void wifi_transmit_height(float height) {
  Enes100.mission(HEIGHT, height);
}

#endif
