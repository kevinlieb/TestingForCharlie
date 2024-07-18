#include "arduino.h"
#include "SpeedoTypeGauge.hpp"


SpeedoTypeGauge gauge;

void setup() {
    char *units = "MPH\0";
    gauge.speedoSetup();
    gauge.setUnits(units);
    gauge.setShowNumberInMiddle(true);
    gauge.setMaxValue(120);
    gauge.setBackgroundColor(TFT_BLACK);
    gauge.setSweep(true);
}

void loop() {
    gauge.speedoLoop();
}