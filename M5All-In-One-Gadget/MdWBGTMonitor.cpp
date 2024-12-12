#include "MdWBGTMonitor.h"
#include "DrTHSensor.h"
#include "M5All-In-One-Gadget.h"
#include <M5Stack.h>

DrTHSensor dthsen;

void MdWBGTMonitor::init()
{
    dthsen.init();
}
void MdWBGTMonitor::getWBGT(double* temperature, double* humidity, WbgtIndex* index)
{
    dthsen.getTempHumi(double* temperature, double* humidity)
    int calc_index = 0.68 * temperature + 0.12 * humidity;

}