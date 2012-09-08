/* Tide_switch
 Copyright (C) 2012 Luke Miller
 
 This code calculates the current tide height for the 
 pre-programmed site. It requires a real time clock
 (DS1307 or DS3231 chips) to generate a time for the 
 calculation.
 
 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program.  If not, see http://www.gnu.org/licenses/.
 
 
 This program is designed to calculate the current tide height
 and control a motor that changes the water level in a tank.
 
 Written under v1.0.1 of the Arduino IDE.
 
 The harmonics constants for the tide prediction are taken from 
 the XTide harmonics file. The original harmonics.tcd file is 
 available at 
 http://www.flaterco.com/xtide/files.html
 As with XTide, the predictions generated by this program should
 not be used for navigation, and no accuracy or warranty is given
 or implied for these tide predictions. Hell, the chances are 
 pretty good that the tide predictions generated here are 
 completely wrong.
 
 
 */
//********************************************************************************

// Initial setup
//*******************************
// Header files for talking to real time clock
#include <Wire.h>
#include <SPI.h>  // Required for RTClib to compile properly
#include <RTClib.h> // From https://github.com/MrAlvin/RTClib
// Real Time Clock setup
RTC_DS3231 RTC;   
// RTC_DS1307 RTC;  // Uncomment this version if you use the older DS1307 clock
/* Real Time clock connections:
   For newer Arduino Uno R3 or Leonardo boards, connect the SDA and SCL lines from
   the Arduino to the SDA and SCL lines on the real time clock board. Also attach
   ground and +5V from the Arduino to the real time clock board. On older Arduino
   boards without the SDA/SCL pins, connect Arduino analog pin 4 to SDA on the 
   clock, and analog pin 5 to the SCL pin on the clock.
*/

// Tide calculation library setup. A folder of the same name containing
// the appropriate .h and .cpp files must be installed in the 
// arduino-1.x.x/libraries folder. 
// Change this library name to use a different site.
#include "TidelibPortSanLuis.h"

TideCalc myTideCalc;  // Create TideCalc object called myTideCalc


int currMinute; // Keep track of current minute value in main loop
float results;  // results holds the output from the tide calc. Units = ft.

//********************************
// This is the level at which the tide will switch from 
// high to low and vice-versa as the predicted tide height
// rises and falls. Changing this will affect how long your
// tank is submerged and empty. 
float virtualShoreHeight = 2.710;   // Units = feet
//*********************************
boolean HighFlag = false;  // Set true when tide is above virtualShoreLevel
boolean LowFlag = false; // Set true when tide is below virturalShoreLevel

//-----------------------------------------
const int Relay1 = 4;  // Pin number for first relay output. Raises tide.
const int Relay2 = 5;  // Pin number for 2nd relay output. Lowers tide.


//**************************************************************************
// Welcome to the setup loop
void setup(void)
{ 
  Wire.begin();
  RTC.begin();

  // For debugging output to serial monitor
  Serial.begin(115200);
  // Wait 10 seconds to see if user opens a Serial monitor
  // otherwise just continue running without it. 
  if (!Serial) delay(10000);

  //************************************
  // Initialize output pins
  pinMode(Relay1, OUTPUT); //Establish that Relay1 is an output
  digitalWrite(Relay1, LOW); // Set output signal low (i.e. off)
  pinMode(Relay2, OUTPUT);
  digitalWrite(Relay2, LOW); 
  //************************************

  DateTime now = RTC.now();
  currMinute = now.minute(); // Store current minute value
  printTime(now);  // Call printTime function to print date/time to serial
  Serial.println("Calculating tides for: ");
  Serial.println(myTideCalc.returnStationID());
  delay(2000);
}  // End of setup loop.

//**************************************************************************
// Welcome to the Main loop
void loop(void)
{
  // Get current time, store in object 'now'
  DateTime now = RTC.now();

  // If it is the start of a new minute, calculate new tide height and
  // adjust motor position
  if (now.minute() != currMinute) {
    // If now.minute doesn't equal currMinute, a new minute has turned
    // over, so it's time to update the tide height. We only want to do
    // this once per minute. 
    currMinute = now.minute();                   // update currMinute

    Serial.println();
    printTime(now); 

    // Calculate new tide height based on current time
    results = myTideCalc.currentTide(now);
    
    //********************************
    // For debugging
    Serial.println(myTideCalc.returnStationID());
    Serial.print("Tide height: ");
    Serial.print(results, 3);
    Serial.println(" ft.");
    Serial.println(); // blank line
    
    //**********************************
    // Example code to actuate relays. This
    // code can activate a relay for a set 
    // period of time (6 seconds = 6000 ms).
    // Actuate drain valves if the tide height
    // passes the virtualShoreHeight threshold.
    // The additional check of HighFlag and LowFlag
    // ensures that the relays are only actuated once
    // per high or low tide cycle, instead of every 
    // minute. 
    if ( (results > virtualShoreHeight) && !HighFlag) {
      // Tide height is above virtualShoreHeight
      digitalWrite(Relay1, HIGH); // Turn on relay
      delay(6000);                // Wait 6 seconds (6000ms)
      digitalWrite(Relay1, LOW);  // Turn relay back off
      HighFlag = true;            // Set flag if not already set
      LowFlag = false;            // Set flag if not already set
    } 
    else if ( (results <= virtualShoreHeight) && !LowFlag) {
      // Tide height is below virtualShoreHeight
      digitalWrite(Relay2, HIGH);  // Turn on relay
      delay(6000);                 // Wait 6 seconds
      digitalWrite(Relay2, LOW);   // Turn relay back off
      LowFlag = true;              // Set flag if not already set
      HighFlag = false;            // Set flag if not already set
    }
  }    // End of if (now.minute() != currMinute) statement
} // End of main loop


//******************************************************
// Function for printing the current date/time to the 
// serial port in a nicely formatted layout.
void printTime(DateTime now) {
  Serial.print(now.year(), DEC);
  Serial.print('/');
  Serial.print(now.month(), DEC);
  Serial.print('/');
  Serial.print(now.day(), DEC);
  Serial.print(' ');
  Serial.print(now.hour(), DEC);
  Serial.print(':');
  if (now.minute() < 10) {
    Serial.print("0");
    Serial.print(now.minute());
  }
  else if (now.minute() >= 10) {
    Serial.print(now.minute());
  }
  Serial.print(':');
  if (now.second() < 10) {
    Serial.print("0");
    Serial.println(now.second());
  }
  else if (now.second() >= 10) {
    Serial.println(now.second());
  }
}
//********************************************************


