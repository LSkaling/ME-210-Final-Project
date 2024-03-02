#include "Arduino.h"
#include "Drive.h"
#include "L298N.h"

#define BC_FWD 11
#define BC_REV 8
#define BD_FWD 6
#define BD_REV 7
#define AC_FWD 5
#define AC_REV 4
#define AD_FWD 3
#define AD_REV 2

Drive::Drive()
: motor_bc(BC_FWD, BC_REV), 
  motor_bd(BD_FWD, BD_REV), 
  motor_ac(AC_FWD, AC_REV), 
  motor_ad(AD_FWD, AD_REV)
{
    // Constructor body, if additional initialization is required
}

void Drive::drive(int speed, int direction) {
    int motor_ac_power = speed * cos((direction - 135) * PI / 180);
    int motor_ad_power = speed * cos((direction - 225) * PI / 180);
    int motor_bc_power = speed * cos((direction - 45) * PI / 180);
    int motor_bd_power = speed * cos((direction - 315) * PI / 180);

    // Drive the motors
    motor_ac.run(motor_ac_power);
    motor_ad.run(motor_ad_power);
    motor_bc.run(motor_bc_power);
    motor_bd.run(motor_bd_power);
}

void Drive::accelDrive(int creepSpeed, int maxSpeed, int acceleration, int direction, int anticipatedTime) {
    int motor_ac_scalar = cos((direction - 135) * PI / 180);
    int motor_ad_scalar = cos((direction - 225) * PI / 180);
    int motor_bc_scalar = cos((direction - 45) * PI / 180);
    int motor_bd_scalar = cos((direction - 315) * PI / 180);

    // Drive the motors
    motor_ac.setAccel(creepSpeed * motor_ac_scalar, maxSpeed * motor_ac_scalar, acceleration * motor_ac_scalar, anticipatedTime);
    motor_ad.setAccel(creepSpeed * motor_ad_scalar, maxSpeed * motor_ad_scalar, acceleration * motor_ad_scalar, anticipatedTime);
    motor_bc.setAccel(creepSpeed * motor_bc_scalar, maxSpeed * motor_bc_scalar, acceleration * motor_bc_scalar, anticipatedTime);
    motor_bd.setAccel(creepSpeed * motor_bd_scalar, maxSpeed * motor_bd_scalar, acceleration * motor_bd_scalar, anticipatedTime);
}


void Drive::begin_rotate(int speed) {
    motor_ac.run(speed);
    motor_ad.run(speed);
    motor_bc.run(speed);
    motor_bd.run(speed);
}

void Drive::stop() {
    motor_ac.stop();
    motor_ad.stop();
    motor_bc.stop();
    motor_bd.stop();
}

void Drive::rotate(int speed, int time) {
    begin_rotate(speed);
    delay(time);
    stop();

}

void Drive::test() {
    motor_ac.run(100);
    Serial.println("Motor A Forward");
    delay(1000);
    motor_ac.stop();
}