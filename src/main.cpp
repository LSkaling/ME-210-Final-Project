#include <Arduino.h>
#include "Drive.h"
#include "pinSetup.h"
#include <Servo.h>
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
Servo dumper;

enum states{
  INIT, ROTATE, ALIGN, LOAD, GAP_ALIGN, TRAVERSE, DUMP_ALIGN, DUMP, REVERSE_GAP_ALIGN, REVERSE_TRAVERSE, RELOAD, CELEBRATE, WAIT, SHORT_TRAVERSE, CONTACT_ALIGN, CONTACT_PARALLEL, IDLE
};

String stateToString(int state) {
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
    case REVERSE_GAP_ALIGN: return "REVERSE_GAP_ALIGN";
    case REVERSE_TRAVERSE: return "REVERSE_TRAVERSE";
    case RELOAD: return "RELOAD";
    case SHORT_TRAVERSE: return "SHORT_TRAVERSE";
    case CONTACT_ALIGN: return "CONTACT_ALIGN";
    case CONTACT_PARALLEL: return "CONTACT_PARALLEL";
    case IDLE: return "IDLE";
    default: return "UNKNOWN STATE";
  }
}

int state;
int side;
int frontDistance;
int rearDistance;
int CDistance;
int DDistance;
int lapNum;

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
  dumper.write(SERVO_UP);
  drive.begin_rotate(55);
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
    dumper.write(SERVO_DOWN);
    state = DUMP;
  }
}

void handleDump(void){
  if(dumpTimer.check()){
    // analogWrite(PIN_SERVO, 255);
    // analogWrite(PIN_BUZZER, 255);
    lapNum++;
    dumper.write(SERVO_UP);
    drive.drive(150, 20);
    state = REVERSE_GAP_ALIGN;
  }
}

void handleReverseGapAlign(void){
  if(DDistance > 20){
    drive.stop();
    delay(500);
    drive.accelDrive(150, 255, 0.30, 265, 2500);
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
  drive.drive(100, -45);
  delay(300);
  drive.begin_rotate(90);
  for(int i = 0; i < 5; i++){
    dumper.write(SERVO_DOWN);
    delay(500);
    dumper.write(SERVO_UP);
    delay(500);
  }
  drive.stop();
  state = IDLE;
}

void handleWait(void){
  delay(500);
  drive.accelDrive(150, 255, 0.30, 85, 2500);
  state = lapNum < 1 ? SHORT_TRAVERSE : TRAVERSE;
}

void handleShortTraverse(void){
  drive.runAccel();
  if(rearDistance > 85){
    drive.drive(MOTOR_SPEED, 180);
    state = CONTACT_ALIGN;
  }
}

void handleContactAlign(void){
  if(DDistance <= COLLIDE_DISTANCE_THRESHOLD){ 
    drive.drive(MOTOR_SPEED, 100);
    state = CONTACT_PARALLEL;
  }
}

void handleContactParallel(void){
  if(frontDistance <= COLLIDE_DISTANCE_THRESHOLD){
    drive.stop();
    dumpTimer.reset();
    dumper.write(SERVO_DOWN);
    state = DUMP;
  }
}

void handleIdle(void){

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
    case SHORT_TRAVERSE:
      handleShortTraverse();
      break;
    case CONTACT_ALIGN:
      handleContactAlign();
      break;
    case CONTACT_PARALLEL:
      handleContactParallel();
      break;
    case CELEBRATE:
      handleCelebrate();
      break;
    case IDLE:
      handleIdle();
      break;
    default: 
      Serial.println("???");
      break;
  }
}

void setup() {
  state = INIT;
  lapNum = 0;
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
  dumper.attach(PIN_SERVO);
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



