#ifndef MISSION_H
#define MISSION_H

#include "sensors.h"
#include "movement.h"
#include "Enes100.h"
#include <Arduino.h>

/*
 * Functions for completing mission objectives: 
 *    Objective I: finding anomoly
 *    Objective II: fixing anomoly
 *    Objective III: measuring height and length of anomoly
 */


/* 
 * measuring the length of the anomoly
 */
void measure_anomoly() {
  // all measurements are in mm
  double height = 270;
  double length1 = 180;
  double length2 = 135;

  double initial_x = Enes100.getX() * 1000;
  double final_x = 0;
  double length;

  double dist = ultra_get_distance();

  move_to_dist_back(30, 0.05, 100);

  // shift right until the ultrasonic does not see anything
  while (dist < 35) {
    shift_right(80);
    dist = ultra_get_distance();
  }
  stop_motors();

  // calculate the length of the flap
  final_x = Enes100.getX() * 1000;
  length = abs(final_x - initial_x);
  Enes100.print("final_x: ");
  Enes100.println(final_x);
  Enes100.print("initial_x: ");
  Enes100.println(initial_x);
  Enes100.println("length: ");
  Enes100.println(length);

  // determine which measured length is closest to length1 or length2
  if (abs(length1 - length) < abs(length2 - length)) {
    length = length1;
  } else {
    length = length2;
  }

  // transmit the measured height and length
  Enes100.println("Anomoly Measurements (mm)");
  Enes100.print("Height: ");
  Enes100.println(height);
  Enes100.print("Length: ");
  Enes100.println(length);
  // Enes100.println();

  // wifi_transmit_height(height);
  // wifi_transmit_length(length);
}

/* 
 * finding the anomoly
 */
void find_anomoly() {
  if (is_red()) {
    stop_motors();
    Enes100.println("first side is red!");
    // Enes100.println();
  } else {
    // move to other side of crash site
    move_to_dist_back(10, 0.05, 100);
    set_angle_simple(0, 0.05);
    nav_x(100, 0.8, true);
    set_angle_simple(3.1, 0.05);

    // strafe left or right depending on mission site
    if (Enes100.getX() < 1) {
      nav_y(100, 0.3);
    } else {
      nav_y(100, 1.3);
    }

    move_to_dist_for(13.5, 0, 100);
    Enes100.println("second side is red!");
    // Enes100.println();
  }
}

#endif