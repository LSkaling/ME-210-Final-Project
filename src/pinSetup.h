#ifndef PinSetup_h
#define PinSetup_h

#include "Arduino.h"

// Pin definitions
#define PIN_SERVO 9
#define PIN_TRIGGER_D A0
#define PIN_ECHO_D A1
#define PIN_TRIGGER_C A2
#define PIN_ECHO_C A3
#define PIN_TRIGGER_A A4
#define PIN_ECHO_A A5
#define PIN_TRIGGER_B 12
#define PIN_ECHO_B 13
#define PIN_TOGGLE 10
#define SERVO_UP 90
#define SERVO_DOWN 0

#define BC_FWD 11
#define BC_REV 8
#define BD_FWD 6
#define BD_REV 7
#define AC_FWD 5
#define AC_REV 4
#define AD_FWD 3
#define AD_REV 2

void initializePins();

#endif
