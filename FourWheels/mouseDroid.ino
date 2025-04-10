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

  Serial.print("Distance is ");
  Serial.println(distance);
  delay(100);

  for (int i = 0; i < 180; i++){
    sm.write(i);
    Serial.println(i);
  }

//  sm.write(0);
//  int period = 1 * 6000; // .5 minutes
//  for(int start = millis(); (millis()-start) < period;) {
//    
//  }
//  sm.write(180);
//  
//  //int period = .5 * 6000; // .5 minutes
//  for(int start = millis(); (millis()-start) < period;) {
//    
//  }
}

void Turning(){
  float left_data;
  float right_data;
  sm.write(90); //TODO: figure out orientation
  sm.write(-180);
}

//Continuous motors: assuming
//0 is forwards (full-speed)
//180 is backwards (full-speed)
//90 is no speed
void Forwards(){
  fl.write(0);
  bl.write(0);
  fr.write(0);
  br.write(0);
}

void Backwards(){
  fl.write(180);
  bl.write(180);
  fr.write(180);
  br.write(180);
}

void Stop(){
  fl.write(90);
  bl.write(90);
  fr.write(90);
  br.write(90);
}

void Left(){
  fl.write(180);
  bl.write(180);
  fr.write(0);
  br.write(0);
}

void Right(){
  fl.write(0);
  bl.write(0);
  fr.write(180);
  br.write(180);
}

//Gather .5 seconds worth of data -> average for number
