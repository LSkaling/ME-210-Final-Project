#ifndef Motor_h
#define Motor_h

class Motor {
    public:
        Motor(int forwardPin, int reversePin);
        void run(int speed);
        void stop();
        void setAccel(int creepPower, int maxPower, double acceleration, double anticipatedTime, int decelPower);
        void runAccel();
    private:
        int _forwardPin;
        int _reversePin;
        int _creepPower;
        int _decelPower;
        int _maxPower;
        double _acceleration;
        double _anticipatedTime;
        unsigned long motorStartTime;
};

#endif