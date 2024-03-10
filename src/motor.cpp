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
    int mapped_power = abs(power);
    if(mapped_power > 255) mapped_power = 255;

    if(dir){
        analogWrite(_forwardPin, mapped_power);
        digitalWrite(_reversePin, LOW);
    }
    else{
        analogWrite(_forwardPin, 255-mapped_power);
        digitalWrite(_reversePin, HIGH);
    }
}

void Motor::setAccel(int creepPower, int maxPower, double acceleration, double anticipatedTime, int decelPower){ //Accelerate at acceleration rate until halfway, then decelerate at acceleration rate
    //starting conditions
    _creepPower = creepPower;
    _maxPower = maxPower;
    _acceleration = acceleration;
    _anticipatedTime = anticipatedTime;
    _decelPower = decelPower;
    motorStartTime = millis();
}

void Motor::runAccel(){ //Note: does not stop the motor after, keeps it creeping
    unsigned long motorTime = millis() - motorStartTime;
    if(motorTime < _anticipatedTime/2){
        int power = _creepPower + motorTime * _acceleration;
        if(_maxPower > 0){
            power = power > _maxPower ? _maxPower : power;
        }else{
            power = power < _maxPower ? _maxPower : power;
        }
        //Serial.print(power); Serial.print(" "); Serial.print(_creepPower); Serial.print(" "); Serial.print(motorTime); Serial.print(" "); Serial.println(_acceleration);
        Serial.print(power); Serial.print(" "); Serial.println(motorTime);
        run(power);
    }
    else if(motorTime < _anticipatedTime){
        int power = _maxPower - (motorTime - _anticipatedTime/2) * _acceleration;
        if(_maxPower > 0){
            power = power < _decelPower ? _decelPower : power;
        }else{
            power = power > _decelPower ? _decelPower : power;
        }
        Serial.print(power); Serial.print(" "); Serial.println(motorTime);
        run(power);
    }
}


void Motor::stop() {
    digitalWrite(_forwardPin, HIGH);
    digitalWrite(_reversePin, HIGH);
}