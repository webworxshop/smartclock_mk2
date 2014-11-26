BOARD_TAG    = pro328 
ARDUINO_PORT = /dev/ttyUSB*

ARDUINO_LIBS = time/Time time/TimeAlarms SPI Bluetooth pubsubclient/PubSubClient jeelib
ARDUINO_SKETCHBOOK = .

include $(ARDMK_DIR)/Arduino.mk
