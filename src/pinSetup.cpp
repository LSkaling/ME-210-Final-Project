#include "PinSetup.h"

void initializePins() {
    pinMode(PIN_TOGGLE, INPUT);
    pinMode(PIN_SERVO, OUTPUT);
    pinMode(PIN_TRIGGER_A, OUTPUT);
    pinMode(PIN_TRIGGER_B, OUTPUT);
    pinMode(PIN_TRIGGER_C, OUTPUT);
    pinMode(PIN_TRIGGER_D, OUTPUT);
    pinMode(PIN_ECHO_A, INPUT);
    pinMode(PIN_ECHO_B, INPUT);
    pinMode(PIN_ECHO_C, INPUT);
    pinMode(PIN_ECHO_D, INPUT);

    pinMode(BC_FWD, OUTPUT);
    pinMode(BC_REV, OUTPUT);
    pinMode(BD_FWD, OUTPUT);
    pinMode(BD_REV, OUTPUT);
    pinMode(AC_FWD, OUTPUT);
    pinMode(AC_REV, OUTPUT);
    pinMode(AD_FWD, OUTPUT);
    pinMode(AD_REV, OUTPUT);
}
