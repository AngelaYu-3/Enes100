#include "Enes100.h"

void setup() {
  // put your setup code here, to run once:

  // connect digital pin 8 to the Tx pin of the wifi module
  // connect digital pin 9 to the Rx pin of the wifi module
  // test comment
  Enes100.begin("Space Crash", CRASH_SITE, 11, 1116, 8, 9);

}

void loop() {
  // put your main code here, to run repeatedly:
  // Enes100.println("hello world!");
  Enes100.print("x: ");
  Enes100.println(Enes100.getX());
  delay(1000);

  Enes100.print("y: ");
  Enes100.println(Enes100.getY());
  delay(1000);

  Enes100.print("theta: ");
  Enes100.println(Enes100.getTheta());
  delay(1000);

  Enes100.print("isVisible: ");
  Enes100.println(Enes100.isVisible());
  delay(1000);

}

