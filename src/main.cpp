#include <Arduino.h>
#include "Drive.h"
#include "pinSetup.h"

#define COLLIDE_DISTANCE_THRESHOLD 2
#define SIDE_DISTANCE_THRESHOLD 62
#define ROTATE_DISTANCE_THRESHOLD 15
#define MAX_DISTANCE 2000

#include <NewPing.h>
#include <Metro.h>
#define MOTOR_SPEED 40

static Metro loadTimer = Metro(30);
static Metro dumpTimer = Metro(1500);
static Metro distReadings = Metro (10); // interval between distance readings
NewPing sensorA(PIN_TRIGGER_A, PIN_ECHO_A, MAX_DISTANCE);
NewPing sensorB(PIN_TRIGGER_B, PIN_ECHO_B, MAX_DISTANCE);
NewPing sensorC(PIN_TRIGGER_C, PIN_ECHO_C, MAX_DISTANCE);
NewPing sensorD(PIN_TRIGGER_D, PIN_ECHO_D, MAX_DISTANCE);

enum states{
  ROTATE, ALIGN, LOAD, GAP_ALIGN, TRAVERSE, DUMP_ALIGN, DUMP, CELEBRATE, WAIT
};

String stateToString(states state) {
  switch (state) {
    case ROTATE: return "ROTATE";
    case ALIGN: return "ALIGN";
    case LOAD: return "LOAD";
    case GAP_ALIGN: return "GAP_ALIGN";
    case TRAVERSE: return "TRAVERSE";
    case DUMP_ALIGN: return "DUMP_ALIGN";
    case DUMP: return "DUMP";
    case CELEBRATE: return "CELEBRATE";
    case WAIT: return "WAIT";
    default: return "UNKNOWN STATE";
  }
}

int state;
int side;
int frontDistance;
int rearDistance;
int CDistance;
int DDistance;

Drive drive;

void updateUltrasonic(void){
  distReadings.reset();
  CDistance = sensorC.ping_cm();
  DDistance = sensorD.ping_cm();
  if(side){
    frontDistance = sensorA.ping_cm();
    rearDistance = sensorB.ping_cm();
  }
  else{
    frontDistance = sensorB.ping_cm();
    rearDistance = sensorA.ping_cm();
  }
  Serial.println("Front: " + String(frontDistance) + " Rear: " + String(rearDistance) + " C: " + String(CDistance) + " D: " + String(DDistance));
}

void handleRotate(void){
  if(rearDistance <= ROTATE_DISTANCE_THRESHOLD && CDistance <= ROTATE_DISTANCE_THRESHOLD){
    drive.begin_rotate(MOTOR_SPEED);
    state = ALIGN;
  }
}

void handleAlign(void){
  if(rearDistance <= COLLIDE_DISTANCE_THRESHOLD && CDistance <= COLLIDE_DISTANCE_THRESHOLD){
    drive.stop();
    loadTimer.reset();
    state = LOAD;
  }
}

void handleLoad(void){
  if(loadTimer.check()){
    drive.drive(MOTOR_SPEED, 0);
    state = GAP_ALIGN;
  }
}

void handleGapAlign(void){
  drive.drive(100, 200);
  if(DDistance < 60){ //TODO: Only works on one side
    drive.stop();
    state = WAIT;
  }
}

void handleTraverse(void){
  drive.drive(100, 90);
  if(frontDistance <= COLLIDE_DISTANCE_THRESHOLD){
    drive.stop();
    dumpTimer.reset();
    state = DUMP_ALIGN;
  }
}

void handleDumpAlign(void){
  drive.drive(100, 170);
  if(DDistance <= COLLIDE_DISTANCE_THRESHOLD){
    drive.stop();
    state = DUMP;
  }
}

void handleDump(void){
  if(dumpTimer.check()){
    // analogWrite(PIN_SERVO, 255);
    // analogWrite(PIN_BUZZER, 255);
    state = CELEBRATE;
  }
}

void handleCelebrate(void){
  Serial.println("Finished");
}

void handleWait(void){
  delay(500);
  state = TRAVERSE;
}

void handleStateTransitions(void){
  switch(state){
    case ROTATE: 
      handleRotate();
      break;
    case ALIGN:
      handleAlign();
      break;
    case LOAD:
      handleLoad();
      break;
    case GAP_ALIGN:
      handleGapAlign();
      break;
    case TRAVERSE:
      handleTraverse();
      break;
    case DUMP_ALIGN:
      handleDumpAlign();
      break;
    case DUMP:
      handleDump();
      break;
    case WAIT:
      handleWait();
      break;
    default: 
      Serial.println("???");
      break;
  }
}

void setup() {
  state = GAP_ALIGN; //TODO: change to ROTATE
  initializePins();
  //rotCCW(20);
  side = digitalRead(PIN_TOGGLE);
  frontDistance = MAX_DISTANCE;
  rearDistance = MAX_DISTANCE;
  CDistance = MAX_DISTANCE;
  DDistance = MAX_DISTANCE;
  distReadings.reset();
  Serial.begin(9600);
  delay(1000);
  Serial.println("Starting");
  // drive.test();
  // drive.drive(30, 135);
  // delay(5000);
  // drive.stop();
  Serial.println("Done");
}

void loop() {
  if(distReadings.check()){
    updateUltrasonic();
  }
  Serial.println("State: " + stateToString(state));
  handleStateTransitions();
}



