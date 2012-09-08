Tide_switch

Arduino code and libraries for building a tide calculator that can
also drive some external switches or relays.

-----------------------------------------------
Installation:

Copy the RTClib folder and TidelibPortSanLuis folders into your
ardiuno-1.x.x/libraries folder. Move the entire folder, not just the
contents of these folders. 

Copy the RealTimeClock_reset and Tide_switch folders to the 
directory where you store your Arduino files (not in the 
libraries sub-folder). 

----------------------------------------------
Usage:

RealTimeClock_reset
--------------------------
If you have a real time clock chip that hasn't been initialized
with the correct time, you will need to open the RealTimeClock_reset
program with the Arduino.app or Arduino.exe program. Begin by making
sure that your computer's time is set to local standard time, *not*
daylight savings time (Mar - Nov). The tide prediction relies on 
having the clock run in local standard time.

Once you have the
real time clock wired up to the Arduino, connect the Arduino to the 
computer with the USB cable. Upload the RealTimeClock_reset program
to the Arduino, then open the Serial Monitor (upper right of Arduino
window) to see if the clock is reporting the correct time (set the serial 
monitor baud rate to 115200). If the time is correct (within 30 seconds),
open a different Arduino program (such as the Tide_switch program) and
upload that to the Arduino without unplugging anything. This prevents the
clock reset program from running more than once and resetting the clock to
the wrong value. 

Tide_switch
-----------------------
Open this program within the Arduino software. It contains the code to
make tide predictions for one site, and to actuate relays. Upload this 
program to your Arduino, with attached real time clock (ChronoDot), and
it will begin making tide predictions. You can launch the Arduino serial
monitor to view those predictions while the Arduino is attached by USB, 
but they only update once per minute, so be patient. 



