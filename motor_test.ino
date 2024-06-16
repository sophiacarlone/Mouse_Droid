#include <Servo.h>

Servo myservo1;
Servo myservo2;

int motor1 = 9;
int motor2 = 8;

void setup() {
  // put your setup code here, to run once:
  myservo1.attach(motor1);
  myservo2.attach(motor2);
}

void loop() {
  // put your main code here, to run repeatedly:
  myservo1.write(0); //no moment
  myservo2.write(180); //0 is forwards, 180 is backwards
  delay(100000);
}
