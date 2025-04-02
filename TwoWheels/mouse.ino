#include <Servo.h>
#include "NewPing.h"

#define MOTOR_RIGHT 9 //right (180-backwards, 0 -forwards)
#define MOTOR_LEFT 11 //left (0-backwards, 180-forwards)
#define MOTOR_SENSOR 10
#define TRIGGER 13 //distance sensor
#define ECHO 12
#define MAX_DIST 360 //max distance to see up to for distance sensor

NewPing distanceSensor(TRIGGER, ECHO, MAX_DIST);
Servo myservo1;
Servo myservo2;
Servo myservo3;

void ChangeDirection();

void setup() {
  myservo1.attach(MOTOR_RIGHT);
  myservo2.attach(MOTOR_LEFT);
  myservo3.attach(MOTOR_SENSOR);
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
  //int data_array[100];
  int distance_left = 0;
  int distance_right = 0;
  myservo1.write(90);
  myservo2.write(90);
//  delay(1000);
//  myservo3.write(0);
//  distance_left = distanceSensor.ping_cm();
//  myservo3.write(90);
//  distance_right = distanceSensor.ping_cm();
////  for (int i = 0; i < 181; i++){
////    myservo3.write(i);
////    data_array[i] = distanceSensor.ping_cm();
////  }
//  Serial.print("right distance");
//  Serial.println(distance_right);
//  Serial.print("leftt distance");
//  Serial.println(distance_left);
//  if (distance_right <= distance_left){
//    Serial.println("right");
//    for (int i = 0; i < 200; i++){ //TODO: figure out how long to turn
//      myservo1.write(180);
//      myservo2.write(180);
//    }
//  }
//  else{
//    Serial.println("left");
//    for (int i = 0; i < 200; i++){ //TODO: figure out how long to turn
//      myservo1.write(0);
//      myservo2.write(0);
//    }    
//  }
//  delay(1000);
//  myservo3.write(45);
}

/*
 * right: servo 1 - 180, servo 2 - 180
 * forward: servo 1 - 0, servo 2 - 180
 * backward: servo 1 - 180, servo 2 - 0
 * left: servo 1 - 0, servo 2 - 0
 */
