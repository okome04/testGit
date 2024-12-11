#include "DrUltraSonic.h"
#include <Arduino.h>

DrUltraSonic::DrUltraSonic(int echo_pin, int trig_pin)
// 引数で与えられたピン番号を使ってECHOピンを入力、TRIGGERピンを出力に設定する。
{
    m_echo_pin = echo_pin;
    m_trig_pin = trig_pin;
    pinMode(echo_pin, INPUT);
    pinMode(trig_pin, OUTPUT);
}
double DrUltraSonic::measureReturnTime()
// 超音波を発生させてから返ってくるまでの時間を変数 ｔ に格納する。tを返す。
{
    double t;        // 時間の変数

    // 超音波の発生
    digitalWrite(m_trig_pin, HIGH);
    delayMicroseconds(10); // 10us
    digitalWrite(m_trig_pin, LOW);

    // 超音波が返ってくるまでの時間を計測
    t = pulseIn(m_echo_pin, HIGH); // パルスがHIGHである時間を調べる

    return t;
}