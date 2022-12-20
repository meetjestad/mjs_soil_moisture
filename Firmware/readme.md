Arduino sketch
=============

To measure soil mositure the mjs_firmware_soil_moisture sketch needs to be uploaded to the MJS board. 
See the readme for the original mjs_firmware firmware for detailed instructions and a list of dependencies: https://github.com/meetjestad/mjs_firmware. 
This skecth has been tested with the original AVR board

Changes to the original sketch
------------------------------

The mjs_firmware_soil_moisture sketch includes code for reading values from the soil moisture sensor over UART in a simple master-slave configuration. The soil moisture sensor needs to be programmed seperately with the Attiny84_slave sketch. 

The measurement interval has been set to 30 minutes, to stay within TTN fair use policty.

