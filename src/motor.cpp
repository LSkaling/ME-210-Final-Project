// #define BC_FWD 9
// #define BC_REV 8
// #define BD_FWD 6
// #define BD_REV 7
// #define AC_FWD 5
// #define AC_REV 4
// #define AD_FWD 3
// #define AD_REV 2

// void driveForward(int speed){
//   if(side){
//     driveA(speed);
//   }
//   else{
//     driveB(speed);
//   }
// }

// void driveRev(int speed){
//   driveForward(-speed);
// }

// void diagFwdD(int speed){
//   if(side){
//     diagAD(speed);
//   }
//   else{
//     diagBD(speed);
//   }
// }

// void diagRevC(int speed){
//   diagFwdD(-speed);
// }

// void motorSetup() {
//   pinMode(BC_FWD, OUTPUT);
//   pinMode(BC_REV, OUTPUT);
//   pinMode(BD_FWD, OUTPUT);
//   pinMode(BD_REV, OUTPUT);
//   pinMode(AC_FWD, OUTPUT);
//   pinMode(AC_REV, OUTPUT);
//   pinMode(AD_FWD, OUTPUT);
//   pinMode(AD_REV, OUTPUT);
// }

// void driveA(int speed){
//   driveMotor(BC_FWD, BC_REV, speed);
//   driveMotor(BD_FWD, BD_REV, -speed);
//   driveMotor(AC_FWD, AC_REV, speed);
//   driveMotor(AD_FWD, AD_REV, -speed);
// }

// void driveB(int speed){
//   driveA(-speed);
// }

// void driveC(int speed){
//   driveMotor(BC_FWD, BC_REV, speed);
//   driveMotor(BD_FWD, BD_REV, speed);
//   driveMotor(AC_FWD, AC_REV, -speed);
//   driveMotor(AD_FWD, AD_REV, -speed);
// }

// void driveD(int speed){
//   driveC(-speed);
// }

// void diagAC(int speed){
//   driveMotor(BC_FWD, BC_REV, speed);
//   driveMotor(BD_FWD, BD_REV, 0);
//   driveMotor(AC_FWD, AC_REV, 0);
//   driveMotor(AD_FWD, AD_REV, -speed);
// }

// void diagBD(int speed){
//   diagAC(-speed);
// }

// void diagAD(int speed){
//   driveMotor(BC_FWD, BC_REV, 0);
//   driveMotor(BD_FWD, BD_REV, -speed);
//   driveMotor(AC_FWD, AC_REV, speed);
//   driveMotor(AD_FWD, AD_REV, 0);
// }

// void diagBC(int speed){
//   diagAD(-speed);
// }

// void rotCCW(int speed){
//   driveMotor(BC_FWD, BC_REV, speed);
//   driveMotor(BD_FWD, BD_REV, speed);
//   driveMotor(AC_FWD, AC_REV, speed);
//   driveMotor(AD_FWD, AD_REV, speed);
// }

// void rotCW(int speed){
//   rotCCW(-speed);
// }

// void stopMotors(void){
//   digitalWrite(BC_FWD, HIGH);
//   digitalWrite(BC_REV, HIGH);
//   digitalWrite(BD_FWD, HIGH);
//   digitalWrite(BD_REV, HIGH);
//   digitalWrite(AC_FWD, HIGH);
//   digitalWrite(AC_REV, HIGH);
//   digitalWrite(AD_FWD, HIGH);
//   digitalWrite(AD_REV, HIGH);
// }

// void driveMotor(int fwd, int rev, int speed){
//   int dir = speed > 0;
//   int val = map(abs(speed), 0, 100, 0, 255);
//   if(dir){
//     analogWrite(fwd, val);
//     digitalWrite(rev, LOW);
//   }
//   else{
//     analogWrite(fwd, 255-val);
//     digitalWrite(rev, HIGH);
//   }
// }
