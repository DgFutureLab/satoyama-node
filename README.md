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

For the Saboten board: 
Add link to the setup up readme of the saboten (now in tekrice/libs/mighty-1284p-freaklabs)

(Inside the readme file change:
"2. Select Tools > Board > Mighty 1284p 16MHz using Optiboot"
for
"2. Select Tools > Board > [Saboten board name (In Mac Original Mighty 1284p 8MHz??)]"




# Installation
Follow these steps to set up a Satoyama sensor node!
## Get the code
1. Download this repository: $ git clone https://github.com/DgFutureLab/satoyama-node.git
2. Go into the new directory: $ cd satoyama-node
3. Download all the libraries: $ git pull && git submodule init && git submodule update && git submodule status

## Prepare Arduino libraries

1. import satoyama-libs/SatoyamaChibiLib into your Arduino IDE. If you use the standard Arduino GUI, go to Sketch → Import Library → Add Library and select the SatoyamaChibiLib folder.
2. import SdFat library
3. import chibiArduino-master library
4. import PCF2127 library

## Prepare the hardware

1. Attach a sonnar, a temperature sensor and a humidity sensor. The pin numbers are specified in <a href="https://github.com/DgFutureLab/satoyama-node/blob/master/SatoyamaChibiLib/config.h">this file</a> (6 for sonar, 0 for temp/humidity)
2. Connect the board to the computer
 
-In case of the Saboten:
Check that the FTDI Breakout board switch is on 5V



4. set the short address of this collector node by hand using test-saboten/test_saboten.ino
5. load collector/collector.ino into your Saboten board. (Include all the necessary libs from third-party-libs folder)
