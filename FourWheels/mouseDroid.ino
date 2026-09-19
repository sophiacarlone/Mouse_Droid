#include <Servo.h>
#include <TimeLib.h>

// UltraSonic Sensors
#define FORWARD_SENSOR 0
#define FORWARD_SENSOR_TRIGGER  13
#define FORWARD_SENSOR_ECHO     12

#define LEFT_SENSOR 1
#define LEFT_SENSOR_TRIGGER  2
#define LEFT_SENSOR_ECHO     4

#define RIGHT_SENSOR 2
#define RIGHT_SENSOR_TRIGGER  7
#define RIGHT_SENSOR_ECHO     8

// Wheels
#define FRONTLEFT       10
#define BACKLEFT        11
#define FRONTRIGHT      8
#define BACKRIGHT       9
Servo fl; //Continuous
Servo bl; //Continuous
Servo fr; //Continuous
Servo br; //Continuous

// Other Globals and Defines
#define WAITTIME 500
#define TURNTIME 5000
#define LOOKAHEAD 30

int countItr; //number of iterations gone through
int sumDistance; //will hold sum of distances to be averaged
int temp_time; 
int wait_time;

/***************************************************/

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
  temp_time = 0;
  wait_time = 500;

  Serial.begin(115200);
}

/***************************************************/

void loop() {
  int distance = readSensor(FORWARD_SENSOR);
  
  sumDistance += distance;
  countItr++;

  if(millis() - temp_time >= wait_time){
    if(sumDistance/countItr <= LOOKAHEAD){
//        Serial.println(sumDistance/countItr);
        String result = Turning(); //TODO make an uint8 when done debugging
        (result == "LEFT") ? Left() : Right(); 
        wait_time = 5000;
    }
    else{
      Forwards();
      wait_time = 500;
    }
    
    temp_time = millis();
    sumDistance = 0;
    countItr = 0;
  }
  
  delay(100);

}

/***************************************************/

int readSensor(int sensor){
  float timing = 0;
  int sensor_trigger = 0;
  int sensor_echo = 0;
  
  switch(sensor){ 
    case FORWARD_SENSOR:
      sensor_trigger = FORWARD_SENSOR_TRIGGER;
      sensor_echo = FORWARD_SENSOR_ECHO;
      break;
    
    case LEFT_SENSOR:
      sensor_trigger = LEFT_SENSOR_TRIGGER;
      sensor_echo = LEFT_SENSOR_ECHO;
      break;
    
    case RIGHT_SENSOR:
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
  return distance;
}

/***************************************************/

//Checking left then right (based on MY orientation and it must be held)
String Turning(){
  float left_data;
  float right_data;

  //left
  left_data = readSensor(1);
  
  //right
  right_data = readSensor(2);

  Serial.print("left: ");
  Serial.println(left_data); //TODO: when done with debugging, can remove variables
  Serial.print("right: ");
  Serial.println(right_data);

  return (right_data > left_data ? "LEFT" : "RIGHT"); //TODO more readable for now, will prob turn to uint8 later
}

/***************************************************/

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

/***************************************************/

void Backwards(){
  fl.write(180);
  bl.write(180);
  fr.write(0);
  br.write(0);
}

/***************************************************/

void Stop(){
  fl.write(90);
  bl.write(90);
  fr.write(90);
  br.write(90);
}

/***************************************************/

void Left(){
  Serial.println("Left");
  fl.write(180);
  bl.write(180);
  fr.write(180);
  br.write(180);
}

/***************************************************/

void Right(){
  Serial.println("Right");
  fl.write(0);
  bl.write(0);
  fr.write(0);
  br.write(0);
}
