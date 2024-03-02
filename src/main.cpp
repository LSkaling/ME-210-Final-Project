#include <Arduino.h>
#include "Drive.h"
#include "pinSetup.h"

#define COLLIDE_DISTANCE_THRESHOLD 2
#define SIDE_DISTANCE_THRESHOLD 62
#define ROTATE_DISTANCE_THRESHOLD 15
#define MAX_DISTANCE 2000

#include <NewPing.h>
#include <Metro.h>
#define MOTOR_SPEED 150

static Metro loadTimer = Metro(30);
static Metro dumpTimer = Metro(1500);
static Metro distReadings = Metro (10); // interval between distance readings
NewPing sensorA(PIN_TRIGGER_A, PIN_ECHO_A, MAX_DISTANCE);
NewPing sensorB(PIN_TRIGGER_B, PIN_ECHO_B, MAX_DISTANCE);
NewPing sensorC(PIN_TRIGGER_C, PIN_ECHO_C, MAX_DISTANCE);
NewPing sensorD(PIN_TRIGGER_D, PIN_ECHO_D, MAX_DISTANCE);

enum states{
  INIT, ROTATE, ALIGN, LOAD, GAP_ALIGN, TRAVERSE, DUMP_ALIGN, DUMP, REVERSE_GAP_ALIGN, REVERSE_TRAVERSE, RELOAD, CELEBRATE, WAIT
};

String stateToString(states state) {
  switch (state) {
    case INIT: return "INIT";
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

void handleInit(){
  drive.begin_rotate(MOTOR_SPEED);
  state = ROTATE;
}

void handleRotate(void){
  if(rearDistance <= ROTATE_DISTANCE_THRESHOLD && CDistance <= ROTATE_DISTANCE_THRESHOLD){
    drive.drive(MOTOR_SPEED, -45);
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
    drive.drive(100, 200);
    state = GAP_ALIGN;
  }
}

void handleGapAlign(void){
  if(DDistance < 60){ //TODO: Only works on one side
    drive.stop();
    state = WAIT;
  }
}

void handleTraverse(void){
  drive.runAccel();
  if(frontDistance <= COLLIDE_DISTANCE_THRESHOLD){
    drive.drive(100, 170);
    state = DUMP_ALIGN;
  }
}

void handleDumpAlign(void){
  if(DDistance <= COLLIDE_DISTANCE_THRESHOLD){
    drive.stop();
    dumpTimer.reset();
    state = DUMP;
  }
}

void handleDump(void){
  if(dumpTimer.check()){
    // analogWrite(PIN_SERVO, 255);
    // analogWrite(PIN_BUZZER, 255);
    drive.drive(100, 10);
    state = REVERSE_GAP_ALIGN;
  }
}

void handleReverseGapAlign(void){
  if(DDistance > 60){
    drive.accelDrive(150, 255, 0.30, 270, 2500);
    state = REVERSE_TRAVERSE;
  }
}

void handleReverseTraverse(void){
  drive.runAccel();
  if(rearDistance <= COLLIDE_DISTANCE_THRESHOLD){
    drive.drive(100, -10);
    state = ALIGN;
  }
}

void handleReload(void){
  drive.drive(150, 0);
  state = GAP_ALIGN;
}

void handleCelebrate(void){
  Serial.println("Finished");
}

void handleWait(void){
  delay(500);
  drive.accelDrive(150, 255, 0.30, 90, 2500);
  state = TRAVERSE;
}

void handleStateTransitions(void){
  switch(state){
    case INIT:
      handleInit();
      break;
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
    case REVERSE_GAP_ALIGN:
      handleReverseGapAlign();
      break;
    case REVERSE_TRAVERSE:
      handleReverseTraverse();
      break;
    
    default: 
      Serial.println("???");
      break;
  }
}

void setup() {
  state = INIT; //TODO: change to ROTATE
  initializePins();
  //rotCCW(20);
  side = digitalRead(PIN_TOGGLE);
  drive.setSide(side);
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

  // drive.accelDrive(50, 150, 0.30, 0, 1000);
  // while(millis() < 8000){
  //   drive.runAccel();
  // }
  // drive.stop();

  // analogWrite(3, 255); //theory: this is always low.
  // digitalWrite(2, LOW);


  // drive.writeMotor(drive.motor_ad, 100);
  // delay(1000);
  // drive.writeMotor(drive.motor_ad, -100);
  // delay(1000);
  // drive.writeMotor(drive.motor_ad, 0);
  // delay(500);

  Serial.println("Done");
}

void loop() {
  if(distReadings.check()){
    updateUltrasonic();
  }
  Serial.println("State: " + stateToString(state));
  handleStateTransitions();
}



