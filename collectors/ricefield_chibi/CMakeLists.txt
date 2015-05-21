# Path to script folder
set(SATOYAMA_LIBS ${CMAKE_CURRENT_SOURCE_DIR}/../../satoyama-libs)

message(${SATOYAMA_LIBS})

# Path to arduino-cmake
set(CMAKE_ARDUINO_PATH ${SATOYAMA_LIBS}/arduino-cmake)


#set(CMAKE_MODULE_PATH ${CMAKE_ARDUINO_PATH}/cmake/Platform/Arduino.cmake)
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_ARDUINO_PATH}/cmake/ArduinoToolchain.cmake) # Arduino Toolchain


# Set the minimum required version of cmake for your project
cmake_minimum_required(VERSION 2.8)

# Set a variable for commands below
set(PROJECT_NAME chibi_ricefield_node)
add_definitions(-std=c++11) 
link_directories(${SATOYAMA_LIBS}/chibiArduino-master/)
link_directories(${SATOYAMA_LIBS}/SatoyamaChibiLib/)
link_directories(${SATOYAMA_LIBS}/SdFat/)
link_directories(${SATOYAMA_LIBS}/PCF2127/)
link_directories(${SATOYAMA_LIBS}/Humidity_Temperature_Sensor/)
link_directories(${SATOYAMA_LIBS}/NewPing/)
link_directories(${SATOYAMA_LIBS}/StandardCplusplus/)

#link_directories(/Applications/Arduino.app/Contents/Resources/Java/hardware/arduino/variants/standard/)

# Define your project and language
project(${PROJECT_NAME} C CXX)


###################################################################
###   ADD ALL ARDUINO-CMAKE COMMANDS AFTER THIS
###################################################################

#register_hardware_platform(${SATOYAMA_LIBS}/hardware/arduino)
#register_hardware_platform(${SATOYAMA_LIBS}/mighty-1284p-freaklabs)
print_board_list()


set(${PROJECT_NAME}_BOARD pro328)

# Define the source code
set(${PROJECT_NAME}_SRCS ricefield_chibi.cpp)

# Define the port for uploading code to the Arduino
set(${PROJECT_NAME}_PORT /dev/tty.usbserial-A90178B0)

# Surpress compile time warings
add_definitions ("-w")

# Enable output of all compile time warnings
#add_definitions ("-Wall")

# Command to generate code arduino firmware (.hex file)
generate_arduino_firmware(${PROJECT_NAME})
