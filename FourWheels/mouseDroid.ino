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
  int last_time_check = 0;
  int start_time_turn = 0; //turn time
  Serial.println(sumDistance);
  if(clearAhead){
    Forwards();
    if(millis() - last_time_check >= WAITTIME){
      sumDistance = sumDistance / countItr;
      if(sumDistance < LOOKAHEAD){
        clearAhead = false;
        sumDistance = 0;
        countItr = 0;
        turnValue = Turning(); //TODO: change strings to define ints
      }
      last_time_check += WAITTIME;
    }
  }
  
  else{
    if (turnValue == "RIGHT") Right();
    else Left();
    if(millis() - start_time_turn >= TURNTIME){
      sumDistance = sumDistance / countItr;
      if(sumDistance > LOOKAHEAD){
        clearAhead = true;
      }
      start_time_turn += TURNTIME;
    }
  }
  
  delay(100);
  sumDistance = 0;
  countItr = 0;
}


void readSensor(int sensor){
  float timing = 0;
  int sensor_trigger = 0;
  int sensor_echo = 0;
  
  switch(sensor){ 
    case 0:
      sensor_trigger = FORWARD_SENSOR_TRIGGER;
      sensor_echo = FORWARD_SENSOR_ECHO;
      break;
    
    case 1:
      sensor_trigger = LEFT_SENSOR_TRIGGER;
      sensor_echo = LEFT_SENSOR_ECHO;
      break;
    
    case 2:
      sensor_trigger = RIGHT_SENSOR_TRIGGER;
      sensor_echo = RIGHT_SENSOR_ECHO;
      break;
    
    default:
      break;
  }

  digitalWrite(sensor_trigger, LOW);
  delay(2);
  digitalWrite(sensor_trigger, HIGH);
  delay(10);
  digitalWrite(sensor_trigger, LOW);
  
  timing = pulseIn(sensor_echo, HIGH);  
  float distance = (timing * 0.034) / 2;

  sumDistance += distance;
  countItr++;
}

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
//0 is forwards for right wheels and backwards for left wheels (full-speed)
//180 is backwards for right wheels and forwards for left wheels (full-speed)
//90 is no speed
void Forwards(){
  Serial.println("Forwards");
  fl.write(0);
  bl.write(0);
  fr.write(180);
  br.write(180);
}

void Backwards(){
  fl.write(180);
  bl.write(180);
  fr.write(0);
  br.write(0);
}

void Stop(){
  fl.write(90);
  bl.write(90);
  fr.write(90);
  br.write(90);
}

void Left(){
  Serial.println("Left");
  fl.write(180);
  bl.write(180);
  fr.write(180);
  br.write(180);
}

void Right(){
  Serial.println("Right");
  fl.write(0);
  bl.write(0);
  fr.write(0);
  br.write(0);
}

//Gather .5 seconds worth of data -> average for number
