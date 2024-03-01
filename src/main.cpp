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
  ROTATE, ALIGN, LOAD, FORWARD, SIDE, DIAG, PARALLEL, DUMP, CELEBRATE
};
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
}

void handleRotate(void){
  if(rearDistance <= ROTATE_DISTANCE_THRESHOLD && CDistance <= ROTATE_DISTANCE_THRESHOLD){
    drive.begin_rotate(MOTOR_SPEED);
    state = ALIGN;
  }
}

void handleAlign(void){
  if(rearDistance <= COLLIDE_DISTANCE_THRESHOLD && CDistance <= COLLIDE_DISTANCE_THRESHOLD){
    drive.stop_rotate();
    loadTimer.reset();
    state = LOAD;
  }
}

void handleLoad(void){
  if(loadTimer.check()){
    drive.drive(MOTOR_SPEED, 0);
    state = FORWARD;
  }
}

void handleForward(void){
  if(frontDistance <= COLLIDE_DISTANCE_THRESHOLD){
    //driveD(MOTOR_SPEED);
    state = SIDE;
  }
}

void handleSide(void){
  if(DDistance <= SIDE_DISTANCE_THRESHOLD){
    //diagFwdD(MOTOR_SPEED);
    state = DIAG;
  }
}

void handleDiag(void){
  if(DDistance <= COLLIDE_DISTANCE_THRESHOLD){
    //driveForward(MOTOR_SPEED);
    state = PARALLEL;
  }
}

void handleParallel(void){
  if(frontDistance <= COLLIDE_DISTANCE_THRESHOLD && frontDistance != 0){
    //stopMotors();
    analogWrite(PIN_SERVO, 0);
    dumpTimer.reset();
    state = DUMP;
  }
}

void handleDump(void){
  if(dumpTimer.check()){
    analogWrite(PIN_SERVO, 255);
    analogWrite(PIN_BUZZER, 255);
    state = CELEBRATE;
  }
}

void handleCelebrate(void){

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
    case FORWARD:
      handleForward();
      break;
    case SIDE:
      handleSide();
      break;
    case DIAG:
      handleDiag();
      break;
    case PARALLEL:
      handleParallel();
      break;
    case DUMP:
      handleDump();
      break;
    case CELEBRATE:
      handleCelebrate();
      break;
    default: 
      Serial.println("???");
      break;
  }
}

void setup() {
  // put your setup code here, to run once:
  state = ROTATE;
  initializePins();
  //rotCCW(20);
  side = digitalRead(PIN_TOGGLE);
  frontDistance = MAX_DISTANCE;
  rearDistance = MAX_DISTANCE;
  CDistance = MAX_DISTANCE;
  DDistance = MAX_DISTANCE;
  distReadings.reset();
  Serial.begin(9600);
}

void loop() {
  if(distReadings.check()){
    updateUltrasonic();
  }
  handleStateTransitions();
}



