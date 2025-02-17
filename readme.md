##  Simple ESP8266 timer with a 128x64 white OLED SSD1306 I2C/SPI display that will help you focus on your work.

### Changing functions using the BOOT button on the development board:

- 1 x press - reset timer
- 2 x press - change main countdown between 5 and 10 minutes
- 3 x press - change break countdown between 2 and 5 minutes
- 4 x press - start break countdown immediately
- 5 x press - start or stop WiFi module to connect/disconnect to network for which credentials were provided
- The reset button reboots the device

------------

### The lengths of individual timers can be edited by changing the values in the code (globals.cpp)

```C++
int mainTimerMinutes = 5;  // Enter the main countdown time here in minutes (e.g. 5)  
int breakTimerMinutes = 2; // Enter the break time here in minutes (e.g. 2)  
int shortBreakSeconds = 6; // Enter the break time between countdowns here in seconds (e.g.6)
```

------------

### WiFi credentials settings

Change the WiFi network name and its password to the desired ones (config.h)

```C++
#define WIFI_SSID "your_wifi_ssid"
#define WIFI_PASSWORD "your_wifi_password"
```
------------

### Physical connections to the OLED display for ESP32 and ESP8266

### **I2C**

| **Display Pin** | **ESP32 Pin** | **ESP8266 Pin** | **Code Label in `config.h`** |
|-----------------|---------------|-----------------|------------------------------|
| VCC             | 3.3V          | 3.3V            | `VCC`                        |
| GND             | GND           | GND             | `GND`                        |
| **SDA**         | GPIO 21       | D2              | `OLED_SDA`                   |
| **SCL**         | GPIO 22       | D1              | `OLED_SCL`                   |

### **SPI**

| **Display Pin** | **ESP32 Pin**  | **ESP8266 Pin**   | **Code Label in `config.h`** |
|-----------------|----------------|-------------------|------------------------------|
| VCC             | 3.3V           | 3.3V              | `VCC`                        |
| GND             | GND            | GND               | `GND`                        |
| **CLK**         | GPIO 18        | GPIO 14 (D5)      | `OLED_CLK`                   |
| **MOSI**        | GPIO 23        | GPIO 13 (D7)      | `OLED_MOSI`                  |
| **CS**          | GPIO 5         | GPIO 15 (D8)      | `OLED_CS`                    |
| **D/C**         | GPIO 17        | GPIO 5 (D1)       | `OLED_DC`                    |

Important Notes:

Remember that the ESP8266 is a 3.3V device; make sure your display is compatible.



