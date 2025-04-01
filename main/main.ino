#include "movement.h"
#include "sensors.h"

void setup() {
  // put your setup code here, to run once:
  delay(1000);
  Serial.begin(9600);
  test();
  Serial.println("setup print");
}

void loop() {
  // put your main code here, to run repeatedly:
}
