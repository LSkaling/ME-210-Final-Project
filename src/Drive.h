#ifndef Drive_h
#define Drive_h

#include "L298N.h"

class Drive {
    public:
        Drive();
        void drive(int speed, int direction);
        void rotate(int speed, int time);
        void begin_rotate(int speed);
        void stop_rotate();
    private:
        L298N motor_bc;
        L298N motor_bd;
        L298N motor_ac;
        L298N motor_ad;

    
};
#endif
    