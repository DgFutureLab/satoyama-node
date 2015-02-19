satoyama-node
=============

Satoyama Sensor Network node stack. This stack is designed to connect chibis and sabotens to <a href="https://github.com/DgFutureLab/satoyama-api">Satoyama API</a>. Created by <a href="http://fljapan.com/">Future Lab</a> based on <a href="http://www.freaklabsstore.com">Freaklabs open hardware</a>.

# Prerequisites

## Hardware
(List of provided parts: board, sensors, batteries, etc.)

(NOTE: Add mark on saboten board to connect the FTDI Breakout board in the right direction!!)

## Software
1. Arduino IDE 1.0.6 (Does not work with Arduino 1.5.8 BETA). Get it <a href="http://www.arduino.cc/">here</a>
2. git

# Installation
Follow these steps to set up a Satoyama sensor node!
## Get the code
1. Download this repository: $ git clone https://github.com/DgFutureLab/satoyama-node.git
2. Go into the new directory: $ cd satoyama-node
3. Download all the libraries: $ git pull && git submodule init && git submodule update && git submodule status

## Prepare Arduino libraries

Import the required libraries into your Arduino IDE:  

1. satoyama-libs/SatoyamaChibiLib 
2. satoyama-libs/SdFat 
3. satoyama-libs/chibiArduino-master
4. satoyama-libs/PCF2127
5. satoyama-libs/Humidity_Temperature_Sensor
6. satoyama-libs/NewPing

If you use the standard Arduino GUI, go to Sketch → Import Library → Add Library and select each library name folder.

For the Saboten board: 

1. Copy the folder 'mighty-1284p-freaklabs' inside satoyama-libs to a folder called 'hardware' off your sketches directory, e.g. /Users/maniacbug/Source/Arduino/hardware/mighty-1284p
In a Mac environment: Arduino.app/contents/resources/Java/hardware/
2. Restart the IDE
3. Select Tools > Board > Freaklabs Saboten...

## Prepare the hardware

1. Attach a sonnar, a temperature sensor and a humidity sensor. The pin numbers are specified in <a href="https://github.com/DgFutureLab/satoyama-node/blob/master/SatoyamaChibiLib/config.h">this file</a> (6 for sonar, 0 for temp/humidity)
2. Connect the board to the computer
 
-In case of the Saboten:
Check that the FTDI Breakout board switch is on 5V

3. For OSX users, download the FTDI drivers from http://www.ftdichip.com/Drivers/VCP.htm


4. Open test_saboten/test_saboten.ino (for saboten)
4. Open File>Examples>Chibiarduino-master>4.cmdline (for Chibi)
5. Select your board on the dropdown menu Tool>Board>(FreakLabs Saboten for the saboten) or (Arduino Pro or ProMini (3.3V, 8MHz) w/ATmega328)
6. Upload it to the board
7. Open Arduino IDE's Serial Monitor
8. Set the communication to Carriage return and 57600 baud
9. Set the short address of this collector node by hand writing   *setsaddr* \<address\>  in the serial monitor command prompt. The address should be in HEXADECIMAL. (The address is currently (as of Jan, 2015) set via the script in satoyama-konohana) 
9. Upload collector/collector.ino into your Saboten board. (Include all the necessary libs from third-party-libs folder)

## Setup build system with CMake
Before we wrote the sketches using the Arduino GUI, but now we use standard CMake to generate make files.

1. Install CMake (e.g. brew install cmake on OSX)
2. Change directory into the folder containing the source code that you want to run on the board.
3. mkdir build (all the build files will be stored in this directory)
4. Navigate to build directory: cd build 
5. compile source code and upload to the device: cmake .. -Wno-dev && make && make upload (make sure that the build directory is empty, as CMake will complain otherwise)
6. Use screen to monitor output from the serial device (e.g. tty.usbserial-A501K9HW): /dev/tty.usbserial-A501K9HW 57600,sc8
