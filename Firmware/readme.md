Arduino sketch
=============

To measure soil mositure the mjs_firmware_soil_moisture_A.ino sketch needs to be uploaded to the MJS board. 
See the readme for the original mjs_firmware firmware for detailed instructions and a list of dependencies: https://github.com/meetjestad/mjs_firmware

This sketch works with the MJS_PROTO_3 board as well as the original AVR board. 
Both the boards require additional components to be added to the experimtal area.
For the AVR board, to ensure proper readings, from the sensors, additionally some resistances need to be removed.
These are R9-R13. This means the original board can no longer be used for Lux, PAR and EC measurements. 





