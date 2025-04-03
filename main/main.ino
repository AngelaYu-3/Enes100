#include "movement.h"
#include "sensors.h"

uint32_t interval = 1000;

void setup() {
  // put your setup code here, to run once:
  delay(1000);
  Serial.begin(9600);
  ultra_setup();
  color_setup();
  //motor_setup();
}

void loop() {
  ultra_get_distance();
  delay(1000);
  is_red();
  // put your main code here, to run repeatedly:
    /*control_motor_A(100, true);
    delay(1000);
    control_motor_A(100, false);
    delay(1000);
    stop_motor_A();*/
}
