#include "arduino.h"
#include "SpeedoTypeGauge.hpp"


SpeedoTypeGauge gauge;

void setup() {
    gauge.speedoSetup();
}

void loop() {
    gauge.speedoLoop();
}