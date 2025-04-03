//#include <HCSR04.h>
//#include

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
  Serial.print("Distance: ");
  Serial.println(distance);
  return distance;
}


// color sensor
#define s0 5
#define s1 29
#define s2 6
#define s3 7
#define out 27

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
  // Serial.print("red: ");
  // Serial.println(red);
  delay(20);
  digitalWrite(s3, HIGH);

  digitalWrite(s3, HIGH);
  blue = pulseIn(out, digitalRead(out) == HIGH ? LOW : HIGH);
  // Serial.print("blue: ");
  // Serial.println(blue);
  delay(20);
  digitalWrite(s3, HIGH);

  digitalWrite(s2, HIGH);
  green = pulseIn(out, digitalRead(out) == HIGH ? LOW : HIGH);
  // Serial.print("green: ");
  // Serial.println(green);
  delay(20);
  digitalWrite(s3, HIGH);
}

bool is_red() {
  get_colors();
  Serial.print("isRed: ");
  if ((red >= 10 && red <= 30) &&
      (blue >= 30 && blue <= 90) &&
      (green >= 30 && green <= 90)) {
        Serial.println("true");
        return true;
  } else {
    Serial.println("false");
    return false;
  }
}