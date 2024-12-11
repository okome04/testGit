#include "MdMeasureDistance.h"
#include "M5All-In-One-Gadget.h"
#include <Arduino.h>

DrUltraSonic drul=DrUltraSonic(ECHO_PIN, TRIG_PIN);

double MdMeasureDistance::getDistance()
{
  double distance = 0;
  distance = (double)drul.measureReturnTime() * 340 * (0.000001) * 0.5 * 100; // m→cmなので100倍する

  return distance;
}
