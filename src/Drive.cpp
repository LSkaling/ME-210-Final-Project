#include "Arduino.h"
#include "Drive.h"
#include "L298N.h"

#define BC_FWD 9
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
    int motor_a_power = speed * cos((direction - 45) * PI / 180);
    int motor_d_power = speed * cos((direction - 135) * PI / 180);
    int motor_c_power = speed * cos((direction - 225) * PI / 180);
    int motor_b_power = speed * cos((direction - 315) * PI / 180);
    // Drive the motors
    motor_ac.setSpeed(motor_a_power);
    motor_ad.setSpeed(motor_d_power);
    motor_bc.setSpeed(motor_b_power);
    motor_bd.setSpeed(motor_c_power);
}

void Drive::rotate(int speed, int time) {
    motor_ac.setSpeed(speed);
    motor_ad.setSpeed(speed);
    motor_bc.setSpeed(speed);
    motor_bd.setSpeed(speed);
    delay(time);
    motor_ac.setSpeed(0);
    motor_ad.setSpeed(0);
    motor_bc.setSpeed(0);
    motor_bd.setSpeed(0);
}

void Drive::begin_rotate(int speed) {
    motor_ac.setSpeed(speed);
    motor_ad.setSpeed(speed);
    motor_bc.setSpeed(speed);
    motor_bd.setSpeed(speed);
}

void Drive::stop_rotate() {
    motor_ac.setSpeed(0);
    motor_ad.setSpeed(0);
    motor_bc.setSpeed(0);
    motor_bd.setSpeed(0);
}