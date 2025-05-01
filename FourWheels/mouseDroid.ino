#include <Servo.h>
#include <TimeLib.h>

#define FRONTLEFT 10
#define BACKLEFT 11
#define FRONTRIGHT 8
#define BACKRIGHT 9
#define SENSORTRIGGER 13
#define SENSORECHO 12
#define SENSORMOTOR 7
#define WAITTIME 500
#define TURNTIME 5000
#define LOOKAHEAD 30

Servo fl; //Continuous
Servo bl; //Continuous
Servo fr; //Continuous
Servo br; //Continuous
Servo sm; //Positional

int countItr; //number of iterations gone through
int sumDistance; //will hold sum of distances to be averaged

void setup() {
  fl.attach(FRONTLEFT);
  bl.attach(BACKLEFT);
  fr.attach(FRONTRIGHT);
  br.attach(BACKRIGHT);
  sm.attach(SENSORMOTOR);
  
  pinMode(SENSORTRIGGER, OUTPUT);
  pinMode(SENSORECHO, INPUT);
  digitalWrite(SENSORTRIGGER, LOW);

  sumDistance = 0;
  countItr = 0;
  sm.write(90);

  Serial.begin(9600);
}

void loop() {
  readSensor();

  if(millis() % WAITTIME <= 500){
    sumDistance = sumDistance / countItr;
    if(sumDistance < LOOKAHEAD){
      sumDistance = 0;
      countItr = 0;
      Serial.println(Turning());
    }
    //double check
    delay(100);
    sumDistance = 0;
    countItr = 0;
  }
}


void readSensor(){
  digitalWrite(SENSORTRIGGER, LOW);
  delay(2);
  digitalWrite(SENSORTRIGGER, HIGH);
  delay(10);
  digitalWrite(SENSORTRIGGER, LOW);

  float timing = pulseIn(SENSORECHO, HIGH);
  float distance = (timing * 0.034) / 2;

  sumDistance += distance;
  countItr++;
}


//Checking left then right (based on MY orientation and it must be held)
String Turning(){
  float left_data;
  float right_data;
  
  sm.write(0); //left
  Serial.println("turning left");
  uint32_t period = TURNTIME;
  for(uint32_t start = millis(); (millis()-start) < (period);){
    readSensor();
  }
  left_data = sumDistance/countItr;
  sumDistance = 0;
  countItr = 0;
  
  sm.write(180); //right
  Serial.println("turning right");
  for(uint32_t start = millis(); (millis()-start) < (period);){
    readSensor();
  }
  right_data = sumDistance/countItr;
  sumDistance = 0;
  countItr = 0;

  sm.write(90);
  return (left_data > right_data ? "LEFT" : "RIGHT"); //more readable for now, will prob turn to uint8 later
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
