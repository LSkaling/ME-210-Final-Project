#include "Arduino.h"
#include "Motor.h"

Motor::Motor(int forwardPin, int reversePin) {
    _forwardPin = forwardPin;
    _reversePin = reversePin;
    pinMode(_forwardPin, OUTPUT);
    pinMode(_reversePin, OUTPUT);
}

void Motor::run(int power) {

    int dir = power > 0;
    power = abs(power);
    if(power > 255) power = 255;

    if(dir){
        analogWrite(_forwardPin, power);
        digitalWrite(_reversePin, LOW);
    }
    else{
        analogWrite(_forwardPin, 255-power);
        digitalWrite(_reversePin, HIGH);
    }
}

void Motor::setAccel(int creepPower, int maxPower, int acceleration, double anticipatedTime){ //Accelerate at acceleration rate until halfway, then decelerate at acceleration rate
    //starting conditions
    _creepPower = creepPower;
    _maxPower = maxPower;
    _acceleration = acceleration;
    _anticipatedTime = anticipatedTime;
    motorStartTime = millis();
}

void Motor::runAccel(){ //Note: does not stop the motor after, keeps it creeping
    unsigned long motorTime = millis() - motorStartTime;
    if(motorTime < _anticipatedTime/2){
        int power = _creepPower + motorTime * _acceleration;
        run(power > _maxPower ? _maxPower : power);
    }
    else if(motorTime < _anticipatedTime){
        int power = _maxPower - (motorTime - _anticipatedTime/2) * _acceleration;
        run(power < _creepPower ? _creepPower : power);
    }
}


void Motor::stop() {
    digitalWrite(_forwardPin, HIGH);
    digitalWrite(_reversePin, HIGH);
}