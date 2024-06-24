#include <Servo.h>

Servo myservo1;
Servo myservo2;

int motor1 = 9; //right (180-backwards, 0 -forwards)
int motor2 = 8; //left (0-backwards, 180-forwards)

void setup() {
  // put your setup code here, to run once:
  myservo1.attach(motor1);
  myservo2.attach(motor2);
}

void loop() {
  // put your main code here, to run repeatedly:
  myservo1.write(0); //no moment
  myservo2.write(0); //0 is forwards, 180 is backwards
  delay(1000);
}

/*
 * right: servo 1 - 180, servo 2 - 180
 * forward: servo 1 - 0, servo 2 - 180
 * backward: servo 1 - 180, servo 2 - 0
 * left: servo 1 - 0, servo 2 - 0
 */
