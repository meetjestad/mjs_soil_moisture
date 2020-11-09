Arduino sketch
=============

To measure soil mositure the mjs_firmware_soil_moisture sketch needs to be uploaded to the MJS board. 
See the readme for the original mjs_firmware firmware for detailed instructions and a list of dependencies: https://github.com/meetjestad/mjs_firmware. 

Changes to the original sketch
------------------------------

The mjs_firmware_soil_moisture sketch includes code for reading values from the soil moisture sensor over I2C in a simple master-slave configuration. The soil moisture sensor needs to be programmed seperately with the Attiny84_slave sketch. 

The mjs_firmware_soil_moisture  sketch requires the latest AVR board files to be installed, which include the Wire.setWireTimeout function. The current setup using I2C for communication between the sensor and the mjs board is not ideal and may cause the sketch to hang and drain the batteries, particularly when long cables are used between the board and the sensor. Including the Wire.setWireTimeout function provides a quick fix for now, untill a better communication protocol is implemeted. 

The measurement interval has been set to 30 minutes, to stay within TTN fair use policty.

The ATTINY slave sketch requires the (non standard) arduino library TinyWireS: https://github.com/rambo/TinyWire 

