#include <Servo.h>

#define FRONTLEFT 10
#define BACKLEFT 11
#define FRONTRIGHT 8
#define BACKRIGHT 9
#define SENSORTRIGGER 13
#define SENSORECHO 12
#define SENSORMOTOR 7

Servo fl; //Continuous
Servo bl; //Continuous
Servo fr; //Continuous
Servo br; //Continuous
Servo sm; //Positional

void setup() {
  fl.attach(FRONTLEFT);
  bl.attach(BACKLEFT);
  fr.attach(FRONTRIGHT);
  br.attach(BACKRIGHT);
  sm.attach(SENSORMOTOR);
  pinMode(SENSORTRIGGER, OUTPUT);
  pinMode(SENSORECHO, INPUT);

  digitalWrite(SENSORTRIGGER, LOW);

  Serial.begin(9600);
}

void loop() {
  digitalWrite(SENSORTRIGGER, LOW);
  delay(2);
  digitalWrite(SENSORTRIGGER, HIGH);
  delay(10);
  digitalWrite(SENSORTRIGGER, LOW);

  float timing = pulseIn(SENSORECHO, HIGH);
  float distance = (timing * 0.034) / 2;
//  float distance = timing;

  Serial.print("Distance is ");
  Serial.println(distance);
  delay(100);
}
