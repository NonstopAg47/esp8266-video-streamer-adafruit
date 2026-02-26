# ESP8266 - Wi-Fi to TFT OLED ST7735 Video Streamer
#### Program runs on an ESP8266 and streams images over Wi-Fi from a server to a 1.8" TFT OLED ST7735 screen. I streamed over wifi because ram and flash is limited and I didnt bother buying an sd card reader.
## SETUP
Setup and activate env if preferred

Install [XAMPP 8.0.25](https://sourceforge.net/projects/xampp/files/XAMPP%20Windows/8.0.25/) or 
your preferred server

Install [Python 3.9.5](https://www.python.org/downloads/release/python-395/)

Install [Arduino IDE 2.1.1](https://github.com/arduino/arduino-ide/releases/tag/2.1.1)

Arduino IDE -> File -> Preferences -> Additional boards manager URLs -> https://arduino.esp8266.com/stable/package_esp8266com_index.json

Arduino IDE -> Tools -> Board -> esp8266 -> NodeMCU 1.0 (ESP-12E Module)

Arduino IDE -> Tools -> Upload Speed -> 912600

Arduino IDE -> Tools -> Serial Monitor -> 921600

Arduino IDE -> Tools -> CPU Frequency -> 160 MHz

## MAIN
### PREPROCESSING
Run `preprocess.sh`

Run XAMPP Apache Server or your preferred server

Configure `server_to_array_to_display_split_files_3/config.h`

Run `server_to_array_to_display_split_files_3/server_to_array_to_display_split_files_3.ino`

## NOTE
> I use git bash on windows to run .sh
>
> Delete temp directory after running `preprocess.sh`
>
> URL in `server_to_array_to_display_split_files_3/server_to_array_to_display_split_files_3.ino` follows the format `http://domain-url/testingimage/00001"`. Ensure folder name matches in server or modify the format


### To do

Check for faster lib - tft_espi ?
