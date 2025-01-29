##  A simple ESP8266 timer with a 128x64 white OLED SSD1306 SPI display that will help you focus on your work.

##### Changing functions using the BOOT button on the development board:

- 1 x press - reset timer
- 2 x press - change main countdown between 5 and 10 minutes
- 3 x press - change break countdown between 2 and 5 minutes
- 4 x press - start break countdown immediately
- The reset button reboots the device

------------

##### The lengths of individual timers can be edited by changing the values in the code
```C
mainTimerMinutes = 5;  // Enter the main countdown time here in minutes (e.g. 5)  
breakTimerMinutes = 2; // Enter the break time here in minutes (e.g. 2)  
shortBreakSeconds = 6; // Enter the break time between countdowns here in seconds (e.g.6)  