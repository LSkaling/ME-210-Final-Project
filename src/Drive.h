#ifndef Drive_h
#define Drive_h

#include "Motor.h"

class Drive {
    public:
        Drive();
        void drive(int speed, int direction);
        void rotate(int speed, int time);
        void begin_rotate(int speed);
        void stop();
        void test();
    private:
        Motor motor_bc;
        Motor motor_bd;
        Motor motor_ac;
        Motor motor_ad;

    
};
#endif
    