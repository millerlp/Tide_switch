/*  TidePortSanLuislib.h 
  A library for calculating the current tide height at 
   Port San Luis, Pacific Ocean, California 
  Luke Miller, June 2012
  https://github.com/millerlp/Tide_calculator
*/ 
 
#ifndef  TidePortSanLuislib_h 
#define  TidePortSanLuislib_h 
#include <Arduino.h>
#include <avr/pgmspace.h>
#include <Wire.h>
#include "RTClib.h"

class TideCalc {
 public:
	 TideCalc();
    float currentTide(DateTime now);
    char* returnStationID(void);
};
#endif