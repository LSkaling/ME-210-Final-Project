#ifndef Motor_h
#define Motor_h

class Motor {
    public:
        Motor(int forwardPin, int reversePin);
        void run(int speed);
        void stop();
        void setAccel(int creepPower, int maxPower, int acceleration, double anticipatedTime);
        void runAccel();
    private:
        int _forwardPin;
        int _reversePin;
        int _creepPower;
        int _maxPower;
        int _acceleration;
        double _anticipatedTime;
        unsigned long motorStartTime;
};

#endif