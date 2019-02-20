## Hardware plans

Created in KiCAD 5.0.2

##### SENSORICNET mother board version 1.0

* Core MCU STM32F072RBT6 (128kB memory)
* Communication by Quectel BC-95 NbIOT modem
* NanoSIM socket
* MOLEX PicoClasp connector for ALPHASENSE OPC-N2 (or OPC-N3) particle meter sensor
* GPS module Antennova M10578-A2, external antenna 
* ATECC508A secure element with internal cryptographic support for data validation, encrypting/decrypting, secure key storage
* RESET button, MODE button, three status LED's, one GPS FIX LED
* Prepared for 6V/500mA power source, variable connector (based on boxing)
* OLED 128x64 SSD1306 BW display
* Pin description added
* Added socket for BME280, it can be placed externally - improved accuracy 

TOP SIDE:

![TOP SIDE](pictures/TOP.png)

BOTTOM SIDE:

![BOTTOM SIDE](pictures/BOTTOM.png)

##### SENSORICNET LORAWAN shield

* Enhancement for SENSORICNET Motherboard
* Lorawan modem Microchip RN2483
* all pins form motherboard fit through this shield, possibility for adding another one

##### SENSORICNET shield template
* Allows anybody to quickly develop own sensing shield for SENSORICNET
