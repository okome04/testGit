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
    int calc_index = 0.68 * (*temperature) + 0.12 * (*humidity);
    Serial.println(calc_index);

/*
    if (calc_index < 15)
    {
        WbgtIndex 変数名 = SAFE;
    }
    else if (calc_index < 24)
    {
        WbgtIndex 変数名 = ATTENTION;
    }
    else if (calc_index < 27)
    {
        WbgtIndex 変数名 = ALERT;
    }
    else if (calc_index < 30)
    {
        WbgtIndex 変数名 = HIGH_ALERT;
    }
    else
    {
        WbgtIndex 変数名 = DANGER;
    }
    */
}