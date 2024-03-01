#ifndef Motor_h
#define Motor_h

class Motor {
    public:
        Motor(int forwardPin, int reversePin);
        void run(int speed);
        void stop();
    private:
        int _forwardPin;
        int _reversePin;
};

#endif