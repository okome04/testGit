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
    dthsen.getTempHumi(temperature, humidity);
    static int calc_index = 0.68 * (*temperature) + 0.12 * (*humidity);
    Serial.println(calc_index);

    if (calc_index < 15)
    {
        *index = SAFE;
    }
    else if (calc_index < 24 && calc_index > 15)
    {
        *index = ATTENTION;
    }
    else if (calc_index < 27 && calc_index > 24)
    {
        *index = ALERT;
    }
    else if (calc_index < 30 && calc_index > 27)
    {
        *index = HIGH_ALERT;
    }
    else if (calc_index > 31)
    {
        *index = DANGER;
    }
    Serial.println(*index);
}