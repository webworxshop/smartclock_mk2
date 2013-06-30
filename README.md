
# SmartClock MK2

An Arduino based smart clock with various sensors
(currently temperature, light and motion). The clock
connects to an MQTT server over a bluetooth connection
for time syncing and upload of sensor data.

## Prerequisites

On Debian GNU/Linux the following packages are required:

 * arduino-core
 * arduino-mk

These will pull in the AVR toolchain as dependancies.
Similar packages should be available for your distribution.

## Firmware Build Instructions

After cloning the repository, first set up the build paths
for arduino-mk. On my Debian system the following exports
will do the trick:

   $ export ARDUINO_DIR=/usr/share/arduino
   $ export ARDMK_DIR=/usr/share/arduino
   $ export AVR_TOOLS_DIR=/usr

Then pull in the dependancies as git submodules:

   $ git submodule init
   $ git submodule update

Then build:

   $ make

To upload to your board, type:

   $ make upload

## Hardware Design

The design uses the Arduino Pro Mini 3.3V [0], but any 
atmega328 based Arduino should work. Detailed build
build instructions will be added to the doc/ directory
soon!

[0] http://www.arduino.cc/en/Main/ArduinoBoardProMini

