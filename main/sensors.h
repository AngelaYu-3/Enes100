#include <HCSR04.h>

// ultrasonic
const int trig_pin = 23;
const int echo_pin = 25;

void ultra_setup() {
  pinMode(trig_pin, OUTPUT);
  pinMode(echo_pin, INPUT);
}

int ultra_get_distance() {
  long duration;
  long distance;

  digitalWrite(trig_pin, LOW);
  delayMicroseconds(2);

  digitalWrite(trig_pin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig_pin, LOW);

  duration = pulseIn(echo_pin, HIGH);
  distance = duration * 0.034 / 2;
  Serial.print("Distance: ");
  Serial.println(distance);
  return distance;
}