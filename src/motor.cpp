#include "Arduino.h"
#include "Motor.h"

Motor::Motor(int forwardPin, int reversePin) {
    _forwardPin = forwardPin;
    _reversePin = reversePin;
    pinMode(_forwardPin, OUTPUT);
    pinMode(_reversePin, OUTPUT);
}

void Motor::run(int speed) {

    int dir = speed > 0;
    int val = map(abs(speed), 0, 100, 0, 255);
    if(dir){
        analogWrite(_forwardPin, val);
        digitalWrite(_reversePin, LOW);
    }
    else{
        analogWrite(_forwardPin, 255-val);
        digitalWrite(_reversePin, HIGH);
    }
}

void Motor::stop() {
    digitalWrite(_forwardPin, HIGH);
    digitalWrite(_reversePin, HIGH);
}