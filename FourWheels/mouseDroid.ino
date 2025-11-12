#include <Servo.h>
#include <TimeLib.h>

#define FRONTLEFT       10
#define BACKLEFT        11
#define FRONTRIGHT      8
#define BACKRIGHT       9

#define FORWARD_SENSOR_TRIGGER  13
#define FORWARD_SENSOR_ECHO     12

#define LEFT_SENSOR_TRIGGER  1
#define LEFT_SENSOR_ECHO     2

#define RIGHT_SENSOR_TRIGGER  3
#define RIGHT_SENSOR_ECHO     4

//TODO rearrange for readability

#define WAITTIME 500
#define TURNTIME 5000
#define LOOKAHEAD 30

Servo fl; //Continuous
Servo bl; //Continuous
Servo fr; //Continuous
Servo br; //Continuous

int countItr; //number of iterations gone through
int sumDistance; //will hold sum of distances to be averaged
boolean clearAhead; //is the path clear of obstacles ahead
String turnValue; //direction of turn

void setup() {
  fl.attach(FRONTLEFT);
  bl.attach(BACKLEFT);
  fr.attach(FRONTRIGHT);
  br.attach(BACKRIGHT);
    
  pinMode(FORWARD_SENSOR_TRIGGER, OUTPUT);
  pinMode(FORWARD_SENSOR_ECHO, INPUT);
  digitalWrite(FORWARD_SENSOR_TRIGGER, LOW);

  pinMode(LEFT_SENSOR_TRIGGER, OUTPUT);
  pinMode(LEFT_SENSOR_ECHO, INPUT);
  digitalWrite(LEFT_SENSOR_TRIGGER, LOW);

  pinMode(RIGHT_SENSOR_TRIGGER, OUTPUT);
  pinMode(RIGHT_SENSOR_ECHO, INPUT);
  digitalWrite(RIGHT_SENSOR_TRIGGER, LOW);

  sumDistance = 0;
  countItr = 0;
  clearAhead = true; //Initially go forwards

  Serial.begin(115200);
}

void loop() {
  readSensor(0);
  Serial.println(sumDistance);
  if(clearAhead){
    Forwards();
    if(millis() % WAITTIME <= 500){
      sumDistance = sumDistance / countItr;
      Serial.println(sumDistance);
      if(sumDistance < LOOKAHEAD){
        clearAhead = false;
        sumDistance = 0;
        countItr = 0;
        //TODO: change strings to define ints
        turnValue = Turning();
      }
    }
  }
  
  else{
    if (turnValue = "RIGHT") Right();
    else Left();
    if(millis() % WAITTIME <= 500){
      sumDistance = sumDistance / countItr;
      if(sumDistance > LOOKAHEAD){
        clearAhead = true;
      }
    }
  }
  
  delay(100);
  sumDistance = 0;
  countItr = 0;
}


void readSensor(int sensor){
  float timing = 0;
  
  switch(sensor){ //TODO: can make these defines
    case 0:
      digitalWrite(FORWARD_SENSOR_TRIGGER, LOW);
      delay(2);
      digitalWrite(FORWARD_SENSOR_TRIGGER, HIGH);
      delay(10);
      digitalWrite(FORWARD_SENSOR_TRIGGER, LOW);
      break;
    case 1:
      digitalWrite(LEFT_SENSOR_TRIGGER, LOW);
      delay(2);
      digitalWrite(LEFT_SENSOR_TRIGGER, HIGH);
      delay(10);
      digitalWrite(LEFT_SENSOR_TRIGGER, LOW);
      break;
    case 2:
      digitalWrite(RIGHT_SENSOR_TRIGGER, LOW);
      delay(2);
      digitalWrite(RIGHT_SENSOR_TRIGGER, HIGH);
      delay(10);
      digitalWrite(RIGHT_SENSOR_TRIGGER, LOW);
      break;
    default:
      break;
  }
  
  timing = pulseIn(FORWARD_SENSOR_ECHO, HIGH);  
  float distance = (timing * 0.034) / 2;

  sumDistance += distance;
  countItr++;
}

//TODO: how are arduinos with multithreading?

//Checking left then right (based on MY orientation and it must be held)
String Turning(){
  float left_data;
  float right_data;
  
  //left
  readSensor(1);
  left_data = sumDistance/countItr;
  sumDistance = 0;
  countItr = 0;
  
  //right
  readSensor(2);
  right_data = sumDistance/countItr;
  sumDistance = 0;
  countItr = 0;

  return (left_data > right_data ? "LEFT" : "RIGHT"); //more readable for now, will prob turn to uint8 later
}

//Continuous motors: assuming
//0 is forwards (full-speed)
//180 is backwards (full-speed)
//90 is no speed
void Forwards(){
  Serial.println("Forwards");
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
