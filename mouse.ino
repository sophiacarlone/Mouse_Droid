#include <Servo.h>
#include "NewPing.h"

#define MOTOR_RIGHT 9 //right (180-backwards, 0 -forwards)
#define MOTOR_LEFT 8 //left (0-backwards, 180-forwards)
#define TRIGGER 10 //distance sensor
#define ECHO 11
#define MAX_DIST 360 //max distance to see up to for distance sensor

NewPing distanceSensor(TRIGGER, ECHO, MAX_DIST);
Servo myservo1;
Servo myservo2;

void ChangeDirection();

void setup() {
  myservo1.attach(MOTOR_RIGHT);
  myservo2.attach(MOTOR_LEFT);
  Serial.begin(9600);
}

void loop() {
  myservo1.write(0); 
  myservo2.write(180); 
  Serial.print("Distance = ");
  int distance = distanceSensor.ping_cm();
  Serial.println(distance);
  
  if((distance < 30) && (distance > 0))
    ChangeDirection(); 

  delay(200);
}

void ChangeDirection(){
  myservo1.write(90);
  myservo2.write(90);
  delay(50000);
}

/*
 * right: servo 1 - 180, servo 2 - 180
 * forward: servo 1 - 0, servo 2 - 180
 * backward: servo 1 - 180, servo 2 - 0
 * left: servo 1 - 0, servo 2 - 0
 */
