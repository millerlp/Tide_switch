Tide_switch

Arduino code and libraries for building a tide calculator that can
also drive some external switches or relays.

-----------------------------------------------
Installation:

Install a copy of RTClib in your Arduino/libraries/ directory. 
If there is not already a libraries directory, create it now. 
RTClib can be downloaded from https://github.com/millerlp/RTClib. 
If the copy you download and unzip is called "RTClib-master", 
rename the folder to simply be "RTClib".

Copy the TidelibPortSanLuis folder into your
Arduino/libraries folder. Move the entire folder, not just the
contents of the folder.


----------------------------------------------
Usage:

RealTimeClock_reset
--------------------------
If the real time clock attached to the Arduino is not yet set, 
you need to set it one time before loading the tide prediction 
software onto the Arduino. Open the Arduino software and go to 
File>Examples>RTClib>settime_exact.ino. Follow the instructions 
at the top of that sketch to upload the correct time to your 
real time clock. Make sure the time you enter is in your local 
standard time, not Daylight Savings Time (which runs Mar-Nov in 
most places). The tide prediction routine relies on the time 
being set to local standard time for your site, otherwise you 
won't get the current tide height out. After running the 
settime_exact.ino sketch, before unplugging the Arduino, 
immediately upload a different sketch (such as the Tide_switch 
sketch) to the Arduino so that the clock doesn't try to reset 
itself repeatedly when the Arduino restarts.

Tide_switch
-----------------------
Open this program within the Arduino software. It contains the code to
make tide predictions for one site, and to actuate relays. Upload this 
program to your Arduino, with attached real time clock (ex: ChronoDot), and
it will begin making tide predictions. You can launch the Arduino serial
monitor to view those predictions while the Arduino is attached by USB, 
but they only update once per minute, so be patient. 



