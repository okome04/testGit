/* インクルードガード */
#pragma once

class DrUltraSonic // classの定義
{
private:
    int m_echo_pin;
    int m_trig_pin;

public:
    //'+ measureReturnTime() : double
    DrUltraSonic(int echo_pin, int trig_pin);
    double measureReturnTime();
};