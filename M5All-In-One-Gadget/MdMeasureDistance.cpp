#include "MdMeasureDistance.h"
#include <MdMeasureDistance.h>
#include <Arduino.h>

double MdMeasureDistance::getDistance()
{
//変数distanceに超音波センサーから取得した値を距離(cm単位)に変換して格納する。distanceを返す。
  double t; //ｔはμ秒単位
  double distance = 0;
  
  /*超音波の発生*/
  digitalWrite(m_trig_pin,HIGH);
  delayMicroseconds(10);
  digitalWrite(m_trig_pin,LOW);

  /*返ってくるまでの時間測定*/
  t = pulseIn(m_echo_pin,HIGH); //ECHOピンのパルスがHIGHになった時間を変数tに格納
  if(t >= 0){
    distance = (double)t * 340*(0.000001)*0.5*100;//m→cmなので100倍する
  }

  return distance;
}


