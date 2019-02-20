EESchema Schematic File Version 4
LIBS:sensenet_particlemeter-cache
EELAYER 26 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L sensenet_particlemeter-rescue:Conn_01x01 J2
U 1 1 59B45EAC
P -1400 6300
F 0 "J2" V -1400 6200 50  0000 C CNN
F 1 "CONN_01X01" H -1000 6250 50  0000 C CNN
F 2 "SNPM:Pin_Header_Straight_1x01_Pitch1.00mm" H -1400 6300 50  0001 C CNN
F 3 "" H -1400 6300 50  0001 C CNN
	1    -1400 6300
	-1   0    0    1   
$EndComp
$Comp
L sensenet_particlemeter-rescue:Conn_01x01 J1
U 1 1 59B45FBA
P -1400 6600
F 0 "J1" V -1400 6500 50  0000 C CNN
F 1 "CONN_01X01" H -1050 6600 50  0000 C CNN
F 2 "SNPM:Pin_Header_Straight_1x01_Pitch1.00mm" H -1400 6600 50  0001 C CNN
F 3 "" H -1400 6600 50  0001 C CNN
	1    -1400 6600
	-1   0    0    1   
$EndComp
$Comp
L sensenet_particlemeter-rescue:Conn_01x01 J7
U 1 1 59B471C1
P -1350 5150
F 0 "J7" H -1250 5150 50  0000 C CNN
F 1 "CONN_01X01" H -850 5150 50  0000 C CNN
F 2 "SNPM:Pin_Header_Straight_1x01_Pitch1.00mm" H -1350 5150 50  0001 C CNN
F 3 "" H -1350 5150 50  0001 C CNN
	1    -1350 5150
	-1   0    0    1   
$EndComp
$Comp
L sensenet_particlemeter-rescue:Conn_01x01 J8
U 1 1 59B4784D
P -1350 5450
F 0 "J8" H -1250 5450 50  0000 C CNN
F 1 "CONN_01X01" H -600 5450 50  0000 C CNN
F 2 "SNPM:Pin_Header_Straight_1x01_Pitch1.00mm" H -1350 5450 50  0001 C CNN
F 3 "" H -1350 5450 50  0001 C CNN
	1    -1350 5450
	-1   0    0    1   
$EndComp
$Comp
L sensenet_particlemeter-rescue:Conn_01x01 J9
U 1 1 59BD7DC6
P 1300 7050
F 0 "J9" H 1300 7150 50  0000 C CNN
F 1 "CONN_01X01" V 1400 7050 50  0000 C CNN
F 2 "SNPM:MountingHole_3.2mm_M3_DIN965" H 1300 7050 50  0001 C CNN
F 3 "" H 1300 7050 50  0001 C CNN
	1    1300 7050
	1    0    0    -1  
$EndComp
$Comp
L sensenet_particlemeter-rescue:Conn_01x01 J10
U 1 1 59BD7F12
P 2100 7050
F 0 "J10" H 2100 7150 50  0000 C CNN
F 1 "CONN_01X01" V 2200 7050 50  0000 C CNN
F 2 "SNPM:MountingHole_3.2mm_M3_DIN965" H 2100 7050 50  0001 C CNN
F 3 "" H 2100 7050 50  0001 C CNN
	1    2100 7050
	1    0    0    -1  
$EndComp
$Comp
L sensenet_particlemeter-rescue:Conn_01x01 J5
U 1 1 5A147BAE
P -1350 4000
F 0 "J5" H -1350 4100 50  0000 C CNN
F 1 "CONN_01X01" H -1050 4000 50  0000 C CNN
F 2 "SNPM:Pin_Header_Straight_1x01_Pitch2.54mm" H -1350 4000 50  0001 C CNN
F 3 "" H -1350 4000 50  0001 C CNN
	1    -1350 4000
	-1   0    0    1   
$EndComp
$Comp
L sensenet_particlemeter-rescue:Conn_01x01 J6
U 1 1 5A147D06
P -1350 4400
F 0 "J6" H -1350 4500 50  0000 C CNN
F 1 "CONN_01X01" H -1000 4400 50  0000 C CNN
F 2 "SNPM:Pin_Header_Straight_1x01_Pitch2.54mm" H -1350 4400 50  0001 C CNN
F 3 "" H -1350 4400 50  0001 C CNN
	1    -1350 4400
	-1   0    0    1   
$EndComp
NoConn ~ 1100 7050
NoConn ~ 1900 7050
$Comp
L sensenet_particlemeter-rescue:Conn_01x01 J4
U 1 1 5A15C8F9
P -1350 3600
F 0 "J4" V -1350 3750 50  0000 C CNN
F 1 "CONN_01X01" V -1350 3250 50  0000 C CNN
F 2 "SNPM:Pin_Header_Straight_1x01_Pitch1.00mm" H -1350 3600 50  0001 C CNN
F 3 "" H -1350 3600 50  0001 C CNN
	1    -1350 3600
	-1   0    0    1   
$EndComp
Text GLabel 7550 1650 2    39   Input ~ 0
Rx
Text GLabel 7550 1750 2    39   Input ~ 0
Tx
Text GLabel 9850 2050 0    39   Input ~ 0
UART_GPS_TX
Text GLabel 10550 2050 2    39   Input ~ 0
UART_GPS_RX
Text GLabel 5950 2050 3    39   Input ~ 0
i2c_sda
Text GLabel 6450 2050 3    39   Input ~ 0
i2c_scl
Text GLabel 6850 1850 0    39   Input ~ 0
SWD_CLK
Text GLabel 6850 1650 0    39   Input ~ 0
SWD_DIO
Text GLabel 10550 2150 2    39   Input ~ 0
WIRELESS_reset
Text GLabel 9850 1950 0    39   Input ~ 0
gps_reset
Text GLabel 10550 1950 2    39   Input ~ 0
gps_standby
Text GLabel 10550 1850 2    39   Input ~ 0
gps_pulses
Text GLabel 9850 2150 0    39   Input ~ 0
gps_fix
Text GLabel 10500 2350 2    39   Input ~ 0
USART_debug_rx
Text GLabel 9850 2350 0    39   Input ~ 0
USART_debug_tx
$Comp
L sensenet_particlemeter-rescue:Conn_01x01 J3
U 1 1 5A148706
P -1350 4800
F 0 "J3" H -1350 4900 50  0000 C CNN
F 1 "CONN_01X01" H -1050 4800 50  0000 C CNN
F 2 "SNPM:Pin_Header_Straight_1x01_Pitch2.54mm" H -1350 4800 50  0001 C CNN
F 3 "" H -1350 4800 50  0001 C CNN
	1    -1350 4800
	-1   0    0    1   
$EndComp
$Comp
L sensenet_particlemeter-rescue:GND #PWR0101
U 1 1 5BA7A8CA
P 7550 1950
F 0 "#PWR0101" H 7550 1700 50  0001 C CNN
F 1 "GND" H 7550 1800 50  0000 C CNN
F 2 "" H 7550 1950 50  0001 C CNN
F 3 "" H 7550 1950 50  0001 C CNN
	1    7550 1950
	0    -1   1    0   
$EndComp
Wire Wire Line
	9850 1950 9950 1950
Wire Wire Line
	9850 2050 9950 2050
Wire Wire Line
	9850 2150 9950 2150
Wire Wire Line
	9850 2350 9950 2350
Wire Wire Line
	10450 1950 10550 1950
Wire Wire Line
	10450 2050 10550 2050
Wire Wire Line
	10450 2150 10550 2150
Wire Wire Line
	10450 2350 10500 2350
Wire Wire Line
	7550 1750 7450 1750
Wire Wire Line
	7550 1850 7450 1850
Wire Wire Line
	7450 1950 7550 1950
Wire Wire Line
	9850 1750 9950 1750
Wire Wire Line
	10450 1750 10500 1750
Wire Wire Line
	9850 1850 9950 1850
Wire Wire Line
	10450 1850 10550 1850
$Comp
L sensenet_particlemeter-rescue:LOGO LOGO1
U 1 1 5BA9EC4C
P 4000 7100
F 0 "LOGO1" H 4000 7300 60  0000 C CNN
F 1 "LOGO" H 4000 6900 60  0000 C CNN
F 2 "graphics:PCB_artwork" H 4000 7100 60  0001 C CNN
F 3 "" H 4000 7100 60  0001 C CNN
	1    4000 7100
	1    0    0    -1  
$EndComp
$Comp
L sensenet_particlemeter-rescue:GND #PWR0102
U 1 1 5BD0C6C7
P 3450 1900
F 0 "#PWR0102" H 3450 1650 50  0001 C CNN
F 1 "GND" H 3450 1750 50  0000 C CNN
F 2 "" H 3450 1900 50  0001 C CNN
F 3 "" H 3450 1900 50  0001 C CNN
	1    3450 1900
	1    0    0    -1  
$EndComp
$Comp
L sensenet_particlemeter-rescue:+5V #PWR0103
U 1 1 5BD0D1A9
P 3350 1750
F 0 "#PWR0103" H 3350 1600 50  0001 C CNN
F 1 "+5V" H 3350 1890 50  0000 C CNN
F 2 "" H 3350 1750 50  0001 C CNN
F 3 "" H 3350 1750 50  0001 C CNN
	1    3350 1750
	0    -1   -1   0   
$EndComp
Text GLabel 3350 1650 0    39   Input ~ 0
PM_SCK
Text GLabel 3350 1550 0    39   Input ~ 0
PM_SDO
Text GLabel 3350 1450 0    39   Input ~ 0
OLED_RESET
Text GLabel 3350 1350 0    39   Input ~ 0
OLED_D/C
Text GLabel 3350 1250 0    39   Input ~ 0
OLED_CS
$Comp
L sensenet_particlemeter-rescue:GND #PWR0104
U 1 1 5C86D82F
P 9850 1750
F 0 "#PWR0104" H 9850 1500 50  0001 C CNN
F 1 "GND" H 9850 1600 50  0000 C CNN
F 2 "" H 9850 1750 50  0001 C CNN
F 3 "" H 9850 1750 50  0001 C CNN
	1    9850 1750
	0    1    1    0   
$EndComp
$Comp
L Connector_Generic:Conn_02x08_Odd_Even J14
U 1 1 5C86E7D2
P 10150 2050
F 0 "J14" H 10200 2567 50  0000 C CNN
F 1 "Conn_02x08_Odd_Even" H 10200 2476 50  0000 C CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_2x08_P2.54mm_Vertical" H 10150 2050 50  0001 C CNN
F 3 "~" H 10150 2050 50  0001 C CNN
	1    10150 2050
	1    0    0    -1  
$EndComp
$Comp
L sensenet_particlemeter-rescue:GND #PWR0105
U 1 1 5C86EDFF
P 10550 2250
F 0 "#PWR0105" H 10550 2000 50  0001 C CNN
F 1 "GND" H 10550 2100 50  0000 C CNN
F 2 "" H 10550 2250 50  0001 C CNN
F 3 "" H 10550 2250 50  0001 C CNN
	1    10550 2250
	0    -1   -1   0   
$EndComp
$Comp
L sensenet_particlemeter-rescue:GND #PWR0106
U 1 1 5C86EED2
P 9850 2250
F 0 "#PWR0106" H 9850 2000 50  0001 C CNN
F 1 "GND" H 9850 2100 50  0000 C CNN
F 2 "" H 9850 2250 50  0001 C CNN
F 3 "" H 9850 2250 50  0001 C CNN
	1    9850 2250
	0    1    1    0   
$EndComp
$Comp
L sensenet_particlemeter-rescue:+5V #PWR0107
U 1 1 5C86F456
P 10550 2450
F 0 "#PWR0107" H 10550 2300 50  0001 C CNN
F 1 "+5V" H 10550 2590 50  0000 C CNN
F 2 "" H 10550 2450 50  0001 C CNN
F 3 "" H 10550 2450 50  0001 C CNN
	1    10550 2450
	0    1    1    0   
$EndComp
Wire Wire Line
	9850 2250 9950 2250
Wire Wire Line
	9850 2450 9950 2450
Wire Wire Line
	10450 2250 10550 2250
Wire Wire Line
	10450 2450 10550 2450
$Comp
L Connector:Conn_01x07_Female J11
U 1 1 5C97EAF5
P 3800 1550
F 0 "J11" H 3827 1576 50  0000 L CNN
F 1 "Conn_01x07_Female" H 3827 1485 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x07_P2.54mm_Vertical" H 3800 1550 50  0001 C CNN
F 3 "~" H 3800 1550 50  0001 C CNN
	1    3800 1550
	1    0    0    -1  
$EndComp
Wire Wire Line
	3350 1250 3600 1250
Wire Wire Line
	3350 1350 3600 1350
Wire Wire Line
	3350 1450 3600 1450
Wire Wire Line
	3350 1550 3600 1550
Wire Wire Line
	3350 1650 3600 1650
Wire Wire Line
	3350 1750 3600 1750
Wire Wire Line
	3450 1900 3450 1850
Wire Wire Line
	3450 1850 3600 1850
$Comp
L sensenet_particlemeter-rescue:+5V #PWR0108
U 1 1 5CAE8A00
P 9850 2450
F 0 "#PWR0108" H 9850 2300 50  0001 C CNN
F 1 "+5V" H 9850 2590 50  0000 C CNN
F 2 "" H 9850 2450 50  0001 C CNN
F 3 "" H 9850 2450 50  0001 C CNN
	1    9850 2450
	0    -1   -1   0   
$EndComp
$Comp
L sensenet_particlemeter-rescue:+3.3V #PWR0109
U 1 1 5CA73288
P 6900 1950
F 0 "#PWR0109" H 6900 1800 50  0001 C CNN
F 1 "+3.3V" H 6900 2090 50  0000 C CNN
F 2 "" H 6900 1950 50  0001 C CNN
F 3 "" H 6900 1950 50  0001 C CNN
	1    6900 1950
	0    -1   1    0   
$EndComp
$Comp
L sensenet_particlemeter-rescue:GND #PWR0110
U 1 1 5CA73793
P 6900 1750
F 0 "#PWR0110" H 6900 1500 50  0001 C CNN
F 1 "GND" H 6900 1600 50  0000 C CNN
F 2 "" H 6900 1750 50  0001 C CNN
F 3 "" H 6900 1750 50  0001 C CNN
	1    6900 1750
	0    1    -1   0   
$EndComp
$Comp
L Connector_Generic:Conn_02x04_Odd_Even J13
U 1 1 5C86D4BC
P 7150 1750
F 0 "J13" H 7200 2067 50  0000 C CNN
F 1 "Conn_02x04_Odd_Even" H 7200 1976 50  0000 C CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_2x04_P2.54mm_Vertical" H 7150 1750 50  0001 C CNN
F 3 "~" H 7150 1750 50  0001 C CNN
	1    7150 1750
	1    0    0    -1  
$EndComp
Wire Wire Line
	5950 2050 5950 1950
Wire Wire Line
	6450 1950 6450 2050
$Comp
L sensenet_particlemeter-rescue:GND #PWR0111
U 1 1 5CB43952
P 7550 1850
F 0 "#PWR0111" H 7550 1600 50  0001 C CNN
F 1 "GND" H 7550 1700 50  0000 C CNN
F 2 "" H 7550 1850 50  0001 C CNN
F 3 "" H 7550 1850 50  0001 C CNN
	1    7550 1850
	0    -1   1    0   
$EndComp
Wire Wire Line
	7550 1650 7450 1650
Wire Wire Line
	6950 1650 6850 1650
Wire Wire Line
	6950 1750 6900 1750
Wire Wire Line
	6950 1850 6850 1850
Wire Wire Line
	6950 1950 6900 1950
$Comp
L sensenet_particlemeter-rescue:GND #PWR0112
U 1 1 5CD6AF96
P 9850 1850
F 0 "#PWR0112" H 9850 1600 50  0001 C CNN
F 1 "GND" H 9850 1700 50  0000 C CNN
F 2 "" H 9850 1850 50  0001 C CNN
F 3 "" H 9850 1850 50  0001 C CNN
	1    9850 1850
	0    1    1    0   
$EndComp
$Comp
L sensenet_particlemeter-rescue:GND #PWR0113
U 1 1 5CD6B055
P 10500 1750
F 0 "#PWR0113" H 10500 1500 50  0001 C CNN
F 1 "GND" H 10500 1600 50  0000 C CNN
F 2 "" H 10500 1750 50  0001 C CNN
F 3 "" H 10500 1750 50  0001 C CNN
	1    10500 1750
	0    -1   -1   0   
$EndComp
$Comp
L Connector_Generic:Conn_02x01 J12
U 1 1 5CB2E8FD
P 6150 1950
F 0 "J12" H 6200 2167 50  0000 C CNN
F 1 "Conn_02x01" H 6200 2076 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x02_P2.54mm_Vertical" H 6150 1950 50  0001 C CNN
F 3 "~" H 6150 1950 50  0001 C CNN
	1    6150 1950
	1    0    0    -1  
$EndComp
Text Notes 2250 1000 0    60   ~ 0
OLED connector for 128x64, driver ssd1306, 7pin
Text Notes 8300 1000 0    60   ~ 0
Shield connector, debug signals
Text Notes 5500 1000 0    60   ~ 0
JTAG, I2C for external bme280
Text Notes 1300 6650 0    60   ~ 0
Mounting holes
Text Notes 3500 6700 0    60   ~ 0
ARTWORK
$EndSCHEMATC
