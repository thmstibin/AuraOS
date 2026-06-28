#include "AuraOS.h"

void setup() {
    AuraOS::instance().begin();
}

void loop() {
    AuraOS::instance().loop();
}
