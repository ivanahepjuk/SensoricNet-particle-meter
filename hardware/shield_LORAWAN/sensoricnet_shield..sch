EESchema Schematic File Version 4
LIBS:sensoricnet_shield.-cache
EELAYER 26 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title "SENSORICNET shield template 1.0"
Date "2019-02-25"
Rev "1.0"
Comp "Labka z.s."
Comment1 "created by i_a"
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L sensenet_particlemeter-rescue:Conn_01x01 J6
U 1 1 59BD7DC6
P 1700 5250
F 0 "J6" H 1700 5350 50  0000 C CNN
F 1 "CONN_01X01" V 1800 5250 50  0000 C CNN
F 2 "SNPM:MountingHole_3.2mm_M3_DIN965" H 1700 5250 50  0001 C CNN
F 3 "" H 1700 5250 50  0001 C CNN
	1    1700 5250
	1    0    0    -1  
$EndComp
$Comp
L sensenet_particlemeter-rescue:Conn_01x01 J5
U 1 1 59BD7F12
P 2300 5250
F 0 "J5" H 2300 5350 50  0000 C CNN
F 1 "CONN_01X01" V 2400 5250 50  0000 C CNN
F 2 "SNPM:MountingHole_3.2mm_M3_DIN965" H 2300 5250 50  0001 C CNN
F 3 "" H 2300 5250 50  0001 C CNN
	1    2300 5250
	1    0    0    -1  
$EndComp
NoConn ~ 1500 5250
NoConn ~ 2100 5250
Text GLabel 5150 1950 2    39   Input ~ 0
Rx
Text GLabel 5150 2050 2    39   Input ~ 0
Tx
Text GLabel 1650 2000 0    39   Input ~ 0
UART_GPS_TX
Text GLabel 2350 2000 2    39   Input ~ 0
UART_GPS_RX
Text GLabel 3550 2350 3    39   Input ~ 0
i2c_sda
Text GLabel 4050 2350 3    39   Input ~ 0
i2c_scl
Text GLabel 4450 2150 0    39   Input ~ 0
SWD_CLK
Text GLabel 4450 1950 0    39   Input ~ 0
SWD_DIO
Text GLabel 2350 2100 2    39   Input ~ 0
WIRELESS_reset
Text GLabel 1650 1900 0    39   Input ~ 0
gps_reset
Text GLabel 2350 1900 2    39   Input ~ 0
gps_standby
Text GLabel 2350 1800 2    39   Input ~ 0
gps_pulses
Text GLabel 1650 2100 0    39   Input ~ 0
gps_fix
$Comp
L sensenet_particlemeter-rescue:GND #PWR066
U 1 1 5B481FD6
P 5100 4950
F 0 "#PWR066" H 5100 4700 50  0001 C CNN
F 1 "GND" H 5100 4800 50  0000 C CNN
F 2 "" H 5100 4950 50  0001 C CNN
F 3 "" H 5100 4950 50  0001 C CNN
	1    5100 4950
	1    0    0    -1  
$EndComp
Text GLabel 2300 2300 2    39   Input ~ 0
USART_debug_rx
Text GLabel 1650 2300 0    39   Input ~ 0
USART_debug_tx
$Comp
L sensenet_particlemeter-rescue:GND #PWR087
U 1 1 5BA7A8CA
P 5150 2250
F 0 "#PWR087" H 5150 2000 50  0001 C CNN
F 1 "GND" H 5150 2100 50  0000 C CNN
F 2 "" H 5150 2250 50  0001 C CNN
F 3 "" H 5150 2250 50  0001 C CNN
	1    5150 2250
	0    -1   1    0   
$EndComp
Wire Wire Line
	1650 1900 1750 1900
Wire Wire Line
	1650 2000 1750 2000
Wire Wire Line
	1650 2100 1750 2100
Wire Wire Line
	1650 2300 1750 2300
Wire Wire Line
	2250 1900 2350 1900
Wire Wire Line
	2250 2000 2350 2000
Wire Wire Line
	2250 2100 2350 2100
Wire Wire Line
	2250 2300 2300 2300
Wire Wire Line
	5150 2050 5050 2050
Wire Wire Line
	5150 2150 5050 2150
Wire Wire Line
	5050 2250 5150 2250
Wire Notes Line
	950  3100 950  750 
Wire Notes Line
	950  3300 5550 3300
Wire Notes Line
	3100 3300 3100 5900
Wire Notes Line
	5550 5900 950  5900
Wire Notes Line
	950  5900 950  3300
Wire Wire Line
	1650 1700 1750 1700
Wire Wire Line
	2250 1700 2300 1700
Wire Wire Line
	1650 1800 1750 1800
Wire Wire Line
	2250 1800 2350 1800
$Comp
L sensenet_particlemeter-rescue:LOGO LOGO1
U 1 1 5BA9EC4C
P 1600 4050
F 0 "LOGO1" H 1600 4250 60  0000 C CNN
F 1 "LOGO" H 1600 3850 60  0000 C CNN
F 2 "graphics:PCB_artwork" H 1600 4050 60  0001 C CNN
F 3 "" H 1600 4050 60  0001 C CNN
	1    1600 4050
	1    0    0    -1  
$EndComp
$Comp
L sensenet_particlemeter-rescue:GND #PWR093
U 1 1 5BD0C6C7
P 7100 2400
F 0 "#PWR093" H 7100 2150 50  0001 C CNN
F 1 "GND" H 7100 2250 50  0000 C CNN
F 2 "" H 7100 2400 50  0001 C CNN
F 3 "" H 7100 2400 50  0001 C CNN
	1    7100 2400
	1    0    0    -1  
$EndComp
$Comp
L sensenet_particlemeter-rescue:+5V #PWR094
U 1 1 5BD0D1A9
P 7000 2250
F 0 "#PWR094" H 7000 2100 50  0001 C CNN
F 1 "+5V" H 7000 2390 50  0000 C CNN
F 2 "" H 7000 2250 50  0001 C CNN
F 3 "" H 7000 2250 50  0001 C CNN
	1    7000 2250
	0    -1   -1   0   
$EndComp
Text GLabel 7000 2150 0    39   Input ~ 0
PM_SCK
Text GLabel 7000 2050 0    39   Input ~ 0
PM_SDO
Text GLabel 7000 1950 0    39   Input ~ 0
OLED_RESET
Text GLabel 7000 1850 0    39   Input ~ 0
OLED_D/C
Text GLabel 7000 1750 0    39   Input ~ 0
OLED_CS
$Comp
L Connector:Conn_01x02_Female J13
U 1 1 5C6AC441
P 4800 4450
F 0 "J13" H 4750 4550 50  0000 C CNN
F 1 "Conn_01x02_Female" V 4900 4050 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x02_P2.54mm_Vertical" H 4800 4450 50  0001 C CNN
F 3 "~" H 4800 4450 50  0001 C CNN
	1    4800 4450
	-1   0    0    1   
$EndComp
$Comp
L sensenet_particlemeter-rescue:GND #PWR0110
U 1 1 5C86D82F
P 1650 1700
F 0 "#PWR0110" H 1650 1450 50  0001 C CNN
F 1 "GND" H 1650 1550 50  0000 C CNN
F 2 "" H 1650 1700 50  0001 C CNN
F 3 "" H 1650 1700 50  0001 C CNN
	1    1650 1700
	0    1    1    0   
$EndComp
$Comp
L Connector_Generic:Conn_02x08_Odd_Even J17
U 1 1 5C86E7D2
P 1950 2000
F 0 "J17" H 2000 2517 50  0000 C CNN
F 1 "Conn_02x08_Odd_Even" H 2000 2426 50  0000 C CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_2x08_P2.54mm_Vertical" H 1950 2000 50  0001 C CNN
F 3 "~" H 1950 2000 50  0001 C CNN
	1    1950 2000
	1    0    0    -1  
$EndComp
$Comp
L sensenet_particlemeter-rescue:GND #PWR0113
U 1 1 5C86EDFF
P 2350 2200
F 0 "#PWR0113" H 2350 1950 50  0001 C CNN
F 1 "GND" H 2350 2050 50  0000 C CNN
F 2 "" H 2350 2200 50  0001 C CNN
F 3 "" H 2350 2200 50  0001 C CNN
	1    2350 2200
	0    -1   -1   0   
$EndComp
$Comp
L sensenet_particlemeter-rescue:GND #PWR0114
U 1 1 5C86EED2
P 1650 2200
F 0 "#PWR0114" H 1650 1950 50  0001 C CNN
F 1 "GND" H 1650 2050 50  0000 C CNN
F 2 "" H 1650 2200 50  0001 C CNN
F 3 "" H 1650 2200 50  0001 C CNN
	1    1650 2200
	0    1    1    0   
$EndComp
$Comp
L sensenet_particlemeter-rescue:+5V #PWR0115
U 1 1 5C86F456
P 2350 2400
F 0 "#PWR0115" H 2350 2250 50  0001 C CNN
F 1 "+5V" H 2350 2540 50  0000 C CNN
F 2 "" H 2350 2400 50  0001 C CNN
F 3 "" H 2350 2400 50  0001 C CNN
	1    2350 2400
	0    1    1    0   
$EndComp
Wire Wire Line
	1650 2200 1750 2200
Wire Wire Line
	1650 2400 1750 2400
Wire Wire Line
	2250 2200 2350 2200
Wire Wire Line
	2250 2400 2350 2400
$Comp
L Connector:Conn_01x07_Female J7
U 1 1 5C97EAF5
P 7450 2050
F 0 "J7" H 7477 2076 50  0000 L CNN
F 1 "Conn_01x07_Female" H 7477 1985 50  0000 L CNN
F 2 "SNPM:OLED" H 7450 2050 50  0001 C CNN
F 3 "~" H 7450 2050 50  0001 C CNN
	1    7450 2050
	1    0    0    1   
$EndComp
Wire Wire Line
	7000 1750 7250 1750
Wire Wire Line
	7000 1850 7250 1850
Wire Wire Line
	7000 1950 7250 1950
Wire Wire Line
	7000 2050 7250 2050
Wire Wire Line
	7000 2150 7250 2150
Wire Wire Line
	7000 2250 7250 2250
Wire Wire Line
	7100 2400 7100 2350
Wire Wire Line
	7100 2350 7250 2350
$Comp
L sensenet_particlemeter-rescue:+5V #PWR0112
U 1 1 5CAE8A00
P 1650 2400
F 0 "#PWR0112" H 1650 2250 50  0001 C CNN
F 1 "+5V" H 1650 2540 50  0000 C CNN
F 2 "" H 1650 2400 50  0001 C CNN
F 3 "" H 1650 2400 50  0001 C CNN
	1    1650 2400
	0    -1   -1   0   
$EndComp
$Comp
L sensenet_particlemeter-rescue:+3.3V #PWR0111
U 1 1 5CA73288
P 4500 2250
F 0 "#PWR0111" H 4500 2100 50  0001 C CNN
F 1 "+3.3V" H 4500 2390 50  0000 C CNN
F 2 "" H 4500 2250 50  0001 C CNN
F 3 "" H 4500 2250 50  0001 C CNN
	1    4500 2250
	0    -1   1    0   
$EndComp
$Comp
L sensenet_particlemeter-rescue:GND #PWR0119
U 1 1 5CA73793
P 4500 2050
F 0 "#PWR0119" H 4500 1800 50  0001 C CNN
F 1 "GND" H 4500 1900 50  0000 C CNN
F 2 "" H 4500 2050 50  0001 C CNN
F 3 "" H 4500 2050 50  0001 C CNN
	1    4500 2050
	0    1    -1   0   
$EndComp
$Comp
L Connector_Generic:Conn_02x04_Odd_Even J20
U 1 1 5C86D4BC
P 4750 2050
F 0 "J20" H 4800 2367 50  0000 C CNN
F 1 "Conn_02x04_Odd_Even" H 4800 2276 50  0000 C CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_2x04_P2.54mm_Vertical" H 4750 2050 50  0001 C CNN
F 3 "~" H 4750 2050 50  0001 C CNN
	1    4750 2050
	1    0    0    -1  
$EndComp
Wire Wire Line
	3550 2350 3550 2250
Wire Wire Line
	4050 2250 4050 2350
$Comp
L sensenet_particlemeter-rescue:GND #PWR0120
U 1 1 5CB43952
P 5150 2150
F 0 "#PWR0120" H 5150 1900 50  0001 C CNN
F 1 "GND" H 5150 2000 50  0000 C CNN
F 2 "" H 5150 2150 50  0001 C CNN
F 3 "" H 5150 2150 50  0001 C CNN
	1    5150 2150
	0    -1   1    0   
$EndComp
Wire Wire Line
	5150 1950 5050 1950
Wire Wire Line
	4550 1950 4450 1950
Wire Wire Line
	4550 2050 4500 2050
Wire Wire Line
	4550 2150 4450 2150
Wire Wire Line
	4550 2250 4500 2250
$Comp
L sensenet_particlemeter-rescue:GND #PWR0121
U 1 1 5CD6AF96
P 1650 1800
F 0 "#PWR0121" H 1650 1550 50  0001 C CNN
F 1 "GND" H 1650 1650 50  0000 C CNN
F 2 "" H 1650 1800 50  0001 C CNN
F 3 "" H 1650 1800 50  0001 C CNN
	1    1650 1800
	0    1    1    0   
$EndComp
$Comp
L sensenet_particlemeter-rescue:GND #PWR0122
U 1 1 5CD6B055
P 2300 1700
F 0 "#PWR0122" H 2300 1450 50  0001 C CNN
F 1 "GND" H 2300 1550 50  0000 C CNN
F 2 "" H 2300 1700 50  0001 C CNN
F 3 "" H 2300 1700 50  0001 C CNN
	1    2300 1700
	0    -1   -1   0   
$EndComp
$Comp
L Connector_Generic:Conn_02x01 J8
U 1 1 5CB2E8FD
P 3750 2250
F 0 "J8" H 3800 2467 50  0000 C CNN
F 1 "Conn_02x01" H 3800 2376 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x02_P2.54mm_Vertical" H 3750 2250 50  0001 C CNN
F 3 "~" H 3750 2250 50  0001 C CNN
	1    3750 2250
	1    0    0    -1  
$EndComp
Text Notes 5750 1050 0    60   ~ 0
OLED connector for 128x64, driver ssd1306, 7pin
Text Notes 1300 1050 0    60   ~ 0
Shield connector, debug signals
Text Notes 3450 1050 0    60   ~ 0
JTAG, I2C for external bme280
Text Notes 1400 4900 0    60   ~ 0
Mounting holes
Text Notes 1350 3700 0    60   ~ 0
ARTWORK
$Comp
L sensenet_particlemeter-rescue:LOGO LOGO2
U 1 1 5C9B9E15
P 2250 4050
F 0 "LOGO2" H 2250 4250 60  0000 C CNN
F 1 "LOGO" H 2250 3850 60  0000 C CNN
F 2 "SNPM:OSHW-Logo2_9.8x8mm_BackSilkScreen" H 2250 4050 60  0001 C CNN
F 3 "" H 2250 4050 60  0001 C CNN
	1    2250 4050
	1    0    0    -1  
$EndComp
Text Notes 2000 3700 0    60   ~ 0
OPEN HARDWARE
$Comp
L Connector:Barrel_Jack_Switch J12
U 1 1 5C770C91
P 3650 4600
F 0 "J12" H 3705 4917 50  0000 C CNN
F 1 "Barrel_Jack_Switch" H 3705 4826 50  0000 C CNN
F 2 "Connector_BarrelJack:BarrelJack_Horizontal" H 3700 4560 50  0001 C CNN
F 3 "~" H 3700 4560 50  0001 C CNN
	1    3650 4600
	1    0    0    -1  
$EndComp
$Comp
L power:+9V #PWR0130
U 1 1 5C771EA1
P 5150 4350
F 0 "#PWR0130" H 5150 4200 50  0001 C CNN
F 1 "+9V" H 5100 4500 50  0000 C CNN
F 2 "" H 5150 4350 50  0001 C CNN
F 3 "" H 5150 4350 50  0001 C CNN
	1    5150 4350
	1    0    0    -1  
$EndComp
Wire Wire Line
	5150 4350 5150 4450
$Comp
L power:+9V #PWR0131
U 1 1 5C7A8760
P 4200 4450
F 0 "#PWR0131" H 4200 4300 50  0001 C CNN
F 1 "+9V" H 4150 4600 50  0000 C CNN
F 2 "" H 4200 4450 50  0001 C CNN
F 3 "" H 4200 4450 50  0001 C CNN
	1    4200 4450
	1    0    0    -1  
$EndComp
$Comp
L sensenet_particlemeter-rescue:GND #PWR0132
U 1 1 5C7A983C
P 4200 5000
F 0 "#PWR0132" H 4200 4750 50  0001 C CNN
F 1 "GND" H 4200 4850 50  0000 C CNN
F 2 "" H 4200 5000 50  0001 C CNN
F 3 "" H 4200 5000 50  0001 C CNN
	1    4200 5000
	1    0    0    -1  
$EndComp
Wire Wire Line
	3950 4500 4200 4500
Wire Wire Line
	4200 4500 4200 4450
Wire Wire Line
	3950 4600 4200 4600
Wire Wire Line
	4200 4600 4200 4700
Wire Wire Line
	3950 4700 4200 4700
Connection ~ 4200 4700
Wire Wire Line
	4200 4700 4200 5000
$Comp
L Connector:Conn_01x02_Female J14
U 1 1 5C84CC5B
P 4800 4850
F 0 "J14" H 4750 4950 50  0000 C CNN
F 1 "Conn_01x02_Female" V 4850 5150 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x02_P2.54mm_Vertical" H 4800 4850 50  0001 C CNN
F 3 "~" H 4800 4850 50  0001 C CNN
	1    4800 4850
	-1   0    0    1   
$EndComp
Wire Wire Line
	5000 4350 5050 4350
Wire Wire Line
	5050 4350 5050 4450
Wire Wire Line
	5050 4450 5150 4450
Wire Wire Line
	5000 4450 5050 4450
Connection ~ 5050 4450
Wire Wire Line
	5000 4750 5100 4750
Wire Wire Line
	5100 4750 5100 4850
Wire Wire Line
	5000 4850 5100 4850
Connection ~ 5100 4850
Wire Wire Line
	5100 4850 5100 4950
Text Notes 3400 3700 0    60   ~ 0
POWER CONNECTORS
Wire Notes Line
	5550 3300 5550 5900
Wire Notes Line
	8400 750  8400 3100
Wire Notes Line
	950  750  8400 750 
Wire Notes Line
	950  3100 8400 3100
$EndSCHEMATC
