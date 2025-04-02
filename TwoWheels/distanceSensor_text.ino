#include "NewPing.h"

//can put these two together
#define TRIGGER 9
#define ECHO 10

#define MAX_DIST 360 //cm (390 is max of sensor)

NewPing distanceSensor(TRIGGER, ECHO, MAX_DIST);

void setup() {
  Serial.begin(9600);
}

void loop() {
  Serial.print("Distance = ");
  Serial.println(distanceSensor.ping_cm());
  delay(200);
}
