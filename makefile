BOARD_TAG    = pro328 
ARDUINO_PORT = /dev/ttyUSB*

ARDUINO_LIBS = Time TimeAlarms SPI Bluetooth pubsubclient/PubSubClient
ARDUINO_SKETCHBOOK = .

include ./Arduino.mk
