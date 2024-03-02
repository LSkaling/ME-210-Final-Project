#ifndef Drive_h
#define Drive_h

#include "Motor.h"

class Drive {
    public:
        Drive();
        void drive(int speed, int direction);
        void accelDrive(int creepSpeed, int maxSpeed, double acceleration, int direction, int anticipatedTime);
        void runAccel();
        void rotate(int speed, int time);
        void begin_rotate(int speed);
        void stop();
        void test();
        Motor motor_bc;
        Motor motor_bd;
        Motor motor_ac;
        Motor motor_ad;
        void writeMotor(Motor motor, int speed);
    private:


    
};
#endif
    