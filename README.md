satoyama-node
=============

Satoyama Sensor Network node stack

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


4. Open test_saboten/test_saboten.ino
5. Upload it to the board
6. Open Arduino IDE's Serial Monitor
7. Set the communication to Carriage return and 57600 baud
8. Set the short address of this collector node by hand writing   *setsaddr* <address\>  in the serial monitor command prompt (The address is currently (as of Jan, 2015) set via the script in satoyama-konohana) 
9. Upload collector/collector.ino into your Saboten board. (Include all the necessary libs from third-party-libs folder)
