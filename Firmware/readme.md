Arduino sketch
=============

To measure soil mositure the mjs_firmware_soil_moisture sketch needs to be uploaded to the MJS board. 
See the readme for the original mjs_firmware firmware for detailed instructions and a list of dependencies: https://github.com/meetjestad/mjs_firmware

### mjs_firmware_soil_moisture_A
This sketch works with the MJS_PROTO_3 board as well as the original AVR board. 
Both the boards require additional components to be added to the experimtal area.
For the AVR board, to ensure proper readings, from the sensors, additionally some resistances need to be removed.
These are R9-R13. This means the original board can no longer be used for Lux, PAR and EC measurements. 
On the MJS_PROTO_3 board the firmware can also be used in combination with air particle measurements. 
For this the SDC30 sensor needs to be defined in the code by uncommenting line 102:
```
// Enable this define when an SPS30 is attached over I²C
#define WITH_SPS30_I2C //OFF to enable room for SOILMOISTURE in output
```
### mjs_firmware_soil_moisture_B
This sketch works with the MJS_PROTO_2 board, as which has a different pin layout. 
As these boards were orgininally deployed for air particle measurements, the SDC30 sensor is defined by default



