/*
  Funkuhr in Worten - written by Steffen Roth 

  Stunden
  Ein     pin = 2
  (Ein)s  pin = 0
  zwei    pin = 20
  drei    pin = 6
  vier    pin = 25
  (f)ünf  pin = 27
  f(ünf)  pin = 30
  sechs   pin = 3
  sieben  pin = 1
  acht    pin = 31
  (n)eun  pin = 7
  n(eun)  pin = 8
  zeh(n)  pin = 9
  el(f)   pin = 29
  Zwoelf  pin = 24

  Verzoegerung
  vor     pin = 4
  nach    pin = 22

  minuten
  fuenf   pin = 19
  zehn    pin = 18
  viertel pin = 21
  zwanzig pin = 23
  halb    pin = 5
  drei(viertel) pin = 17

  zusatz
  Uhr     pin = 26
  Funk    pin = 28
  Es ist  pin = 16
*/

//ShiftPWM.SetOne(pin, startValue);

//latchpin has to be include before shift PWM library
const int ShiftPWM_latchPin = 8;

//Libraries import
#include <ShiftPWM.h>
#include "DCF77.h"
#include "TimeLib.h"
#include "utility/Utils.h"
using namespace Utils;

#define DCF_PIN 2
#define DCF_INTERRUPT 0

time_t prevDisplay = 0;
//time_t time;
DCF77 DCF = DCF77(DCF_PIN,DCF_INTERRUPT);

const bool ShiftPWM_invertOutputs = false;
const bool ShiftPWM_balanceLoad = true;

const int maxBrightness = 175;

void setup(){
  //Serial.begin(9600);

  ShiftPWM.SetAmountOfRegisters(4);
  ShiftPWM.Start(75,220);
  ShiftPWM.SetAll(0);
  
  DCF.Start();
  setSyncInterval(30);
  setSyncProvider(getDCFTime);
  //setSyncProvider(DCF.getTime);

  while(timeStatus()== timeNotSet) {
     //Serial.println("");
     //Serial.print("Funk");
     ShiftPWM.SetOne(28, maxBrightness);
     delay(1000);
  }
  ShiftPWM.SetAll(0);
}

void loop(){
  //DCF
  if( now() != prevDisplay) //update the display only if the time has changed
  {
    prevDisplay = now();
    ShiftPWM.SetAll(0);
    //Serial.println("");
    //Serial.print("Es ist ");
    ShiftPWM.SetOne(16, maxBrightness);
  
    getMinutes(minute());
    getHours(hour(), minute());
    oClock(minute());
  }  
}

//minuten mit toleranz
void getMinutes(int minutes) {
  if(minutes >= (5 - 2) && minutes < (5 + 3)) {
    //Serial.print("Fuenf ");
    ShiftPWM.SetOne(19, maxBrightness);
    getPast(true);    
  } else if(minutes >= (10 - 2) && minutes < (10 + 3)) {
    //Serial.print("Zehn ");
    ShiftPWM.SetOne(18, maxBrightness);
    getPast(true);
  } else if(minutes >= (15 - 2) && minutes < (15 + 3)) {
    //Serial.print("Viertel ");
    ShiftPWM.SetOne(21, maxBrightness);
    getPast(true);
  } else if(minutes >= (20 - 2) && minutes < (20 + 3)) {
    //Serial.print("Zwanzig ");
    ShiftPWM.SetOne(23, maxBrightness);
    getPast(true);
  } else if(minutes >= (25 - 2) && minutes < (25 + 3)) {
    //Serial.print("Fuenf ");
    ShiftPWM.SetOne(19, maxBrightness);
    getPast(false);
    //Serial.print("halb ");
    ShiftPWM.SetOne(5, maxBrightness);
  } else if(minutes >= (30 - 2) && minutes < (30 + 3)) {
    //Serial.print("halb ");
    ShiftPWM.SetOne(5, maxBrightness);
  } else if(minutes >= (35 - 2) && minutes < (35 + 3)) {
    //Serial.print("Fuenf ");
    ShiftPWM.SetOne(19, maxBrightness);
    getPast(true);
    //Serial.print("halb ");
    ShiftPWM.SetOne(5, maxBrightness);
  } else if(minutes >= (40 - 2) && minutes < (40 + 3)) {
    //Serial.print("Zwanzig ");
    ShiftPWM.SetOne(23, maxBrightness);
    getPast(false);
  } else if(minutes >= (45 - 2) && minutes < (45 + 3)) {
    //Serial.print("Drei");
    //Serial.print("viertel ");
    ShiftPWM.SetOne(17, maxBrightness);
    ShiftPWM.SetOne(21, maxBrightness);
  } else if(minutes >= (50 - 2) && minutes < (50 + 3)) {
    //Serial.print("Zehn ");
    ShiftPWM.SetOne(18, maxBrightness);
    getPast(false);
  } else if(minutes >= (55 - 2) && minutes < (55 + 3)) {
    //Serial.print("Fuenf ");
    ShiftPWM.SetOne(19, maxBrightness);
    getPast(false);
  }
}

void getPast(boolean past){
  if(past == true){
    //Serial.print("nach ");
    ShiftPWM.SetOne(22, maxBrightness);
  } else {
    //Serial.print("vor ");
    ShiftPWM.SetOne(4, maxBrightness);
  }
}

//gibt Stunden zurueck 0 Uhr == 12 Uhr
void getHours(int hour, int minutes) {
  int tempHour = hour;  
  
  if(tempHour > 12) {
    tempHour = tempHour - 12;
  }
  
  if(minutes >= (25 - 2)) {
    tempHour = tempHour + 1; 
  }
  
  switch(tempHour) {
    case 0:
      //Serial.print("Zwoelf ");
      ShiftPWM.SetOne(24, maxBrightness);
      break;
    case 1:
      if (minutes > 57 || minutes < 3) {
        //Serial.print("Ein ");
        ShiftPWM.SetOne(2, maxBrightness);
      } else {
        //Serial.print("Eins ");
        ShiftPWM.SetOne(2, maxBrightness);
        ShiftPWM.SetOne(0, maxBrightness);
      }
      break;
    case 2:
      //Serial.print("Zwei ");
      ShiftPWM.SetOne(20, maxBrightness);
      break;
    case 3:
      //Serial.print("Drei ");
      ShiftPWM.SetOne(6, maxBrightness);
      break;
    case 4:
      //Serial.print("Vier ");
      ShiftPWM.SetOne(25, maxBrightness);
      break;
    case 5:
      //Serial.print("F");
      //Serial.print("uenf ");
      ShiftPWM.SetOne(30, maxBrightness);
      ShiftPWM.SetOne(27, maxBrightness);
      break;
    case 6:
      //Serial.print("Sechs ");
      ShiftPWM.SetOne(3, maxBrightness);
      break;
    case 7:
      //Serial.print("Sieben ");
      ShiftPWM.SetOne(1, maxBrightness);
      break;
    case 8:
      //Serial.print("Acht ");
      ShiftPWM.SetOne(31, maxBrightness);
      break;
    case 9:
      //Serial.print("N");
      //Serial.print("eun ");
      ShiftPWM.SetOne(8, maxBrightness);
      ShiftPWM.SetOne(7, maxBrightness);
      break;
    case 10: 
      //Serial.print("Zeh");
      //Serial.print("n ");
      ShiftPWM.SetOne(9, maxBrightness);
      ShiftPWM.SetOne(8, maxBrightness);
      break;
    case 11: 
      //Serial.print("El");
      //Serial.print("f ");
      ShiftPWM.SetOne(29, maxBrightness);
      ShiftPWM.SetOne(30, maxBrightness);
      break;
    case 12:
      //Serial.print("Zwoelf ");
      ShiftPWM.SetOne(24, maxBrightness);
      break;
    default:
      if (minutes > 57 || minutes < 3) {
        //Serial.print("Ein ");
        ShiftPWM.SetOne(2, maxBrightness);
      } else {
        //Serial.print("Eins ");
        ShiftPWM.SetOne(2, maxBrightness);
        ShiftPWM.SetOne(0, maxBrightness);
      }
      break;
  }
}

//das wort -uhr- wird nur zur vollen Stunde angezeigt
void oClock(int minutes) {
  if (minutes > 57 || minutes < 3) {
    //Serial.print("Uhr");
    ShiftPWM.SetOne(26, maxBrightness);
  }
}

unsigned long getDCFTime()
{
  time_t DCFtime = DCF.getTime();
  // indicator fuer erfolgreichen zeit check
  if (DCFtime!=0) {
    //Serial.print("Funk");
    //ShiftPWM.SetOne(13, maxBrightness);
  }
  return DCFtime;
}
