EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title "Bodemvocht sensor multilevel"
Date ""
Rev ""
Comp "Plan B"
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Wire Wire Line
	1700 1500 1700 1450
Text GLabel 2550 1900 2    50   Input ~ 0
NTC_L
Text GLabel 2600 2900 2    50   Input ~ 0
F_OUT
$Comp
L power:GND #PWR0106
U 1 1 5DF64A5F
P 1700 3400
F 0 "#PWR0106" H 1700 3150 50  0001 C CNN
F 1 "GND" H 1705 3227 50  0000 C CNN
F 2 "" H 1700 3400 50  0001 C CNN
F 3 "" H 1700 3400 50  0001 C CNN
	1    1700 3400
	1    0    0    -1  
$EndComp
$Comp
L power:+3.3V #PWR0107
U 1 1 5DF6C293
P 1500 5450
F 0 "#PWR0107" H 1500 5300 50  0001 C CNN
F 1 "+3.3V" V 1500 5600 50  0000 L CNN
F 2 "" H 1500 5450 50  0001 C CNN
F 3 "" H 1500 5450 50  0001 C CNN
	1    1500 5450
	0    1    1    0   
$EndComp
$Comp
L power:GND #PWR0108
U 1 1 5DF6E916
P 1500 5550
F 0 "#PWR0108" H 1500 5300 50  0001 C CNN
F 1 "GND" V 1500 5400 50  0000 R CNN
F 2 "" H 1500 5550 50  0001 C CNN
F 3 "" H 1500 5550 50  0001 C CNN
	1    1500 5550
	0    -1   -1   0   
$EndComp
Text GLabel 1500 5650 2    50   Input ~ 0
SCL
Text GLabel 1500 5750 2    50   Input ~ 0
SDA
Wire Wire Line
	1400 5450 1500 5450
Wire Wire Line
	1500 5550 1400 5550
Wire Wire Line
	1400 5650 1500 5650
Wire Wire Line
	1500 5750 1400 5750
Text GLabel 2900 2400 2    50   Input ~ 0
SDA
Text GLabel 3100 2200 2    50   Input ~ 0
SCL
Wire Notes Line
	900  5150 900  5950
Wire Notes Line
	900  5950 2000 5950
Wire Notes Line
	2000 5950 2000 5150
Wire Notes Line
	2000 5150 900  5150
Text GLabel 2550 1800 2    50   Input ~ 0
NTC_H
Wire Notes Line
	3400 3700 900  3700
Wire Notes Line
	900  3700 900  1100
Wire Notes Line
	900  1100 3400 1100
Text GLabel 2550 2000 2    50   Input ~ 0
A2
Text GLabel 2550 2500 2    50   Input ~ 0
S0
Text GLabel 2550 2800 2    50   Input ~ 0
S1
$Comp
L Connector_Generic:Conn_01x04 J2
U 1 1 5DEE9FC0
P 1200 5550
F 0 "J2" H 1120 5867 50  0000 C CNN
F 1 "I2C input " H 1120 5776 50  0000 C CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_1x04_P2.54mm_Vertical" H 1200 5550 50  0001 C CNN
F 3 "~" H 1200 5550 50  0001 C CNN
	1    1200 5550
	-1   0    0    -1  
$EndComp
$Comp
L power:GND #PWR0111
U 1 1 5E9EC87C
P 1200 4600
F 0 "#PWR0111" H 1200 4350 50  0001 C CNN
F 1 "GND" H 1205 4427 50  0000 C CNN
F 2 "" H 1200 4600 50  0001 C CNN
F 3 "" H 1200 4600 50  0001 C CNN
	1    1200 4600
	1    0    0    -1  
$EndComp
$Comp
L Device:C C1
U 1 1 5E9F21A9
P 1200 4400
F 0 "C1" H 1315 4446 50  0000 L CNN
F 1 "100nF" H 1315 4355 50  0000 L CNN
F 2 "Capacitor_THT:C_Disc_D5.0mm_W2.5mm_P2.50mm" H 1238 4250 50  0001 C CNN
F 3 "~" H 1200 4400 50  0001 C CNN
	1    1200 4400
	1    0    0    -1  
$EndComp
$Comp
L Device:C C2
U 1 1 5E9F2F9F
P 1550 4400
F 0 "C2" H 1665 4446 50  0000 L CNN
F 1 "10u" H 1665 4355 50  0000 L CNN
F 2 "Capacitor_THT:CP_Radial_D5.0mm_P2.50mm" H 1588 4250 50  0001 C CNN
F 3 "~" H 1550 4400 50  0001 C CNN
	1    1550 4400
	1    0    0    -1  
$EndComp
Wire Wire Line
	1550 4250 1200 4250
Wire Wire Line
	1200 4600 1200 4550
Wire Wire Line
	1200 4550 1550 4550
Connection ~ 1200 4550
$Comp
L Device:R R1
U 1 1 5EA00020
P 2850 1800
F 0 "R1" H 2920 1846 50  0000 L CNN
F 1 "3.3k" H 2920 1755 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P10.16mm_Horizontal" V 2780 1800 50  0001 C CNN
F 3 "~" H 2850 1800 50  0001 C CNN
	1    2850 1800
	1    0    0    -1  
$EndComp
$Comp
L Device:R R2
U 1 1 5EA00F4D
P 3050 1800
F 0 "R2" H 3120 1846 50  0000 L CNN
F 1 "3.3k" H 3120 1755 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P10.16mm_Horizontal" V 2980 1800 50  0001 C CNN
F 3 "~" H 3050 1800 50  0001 C CNN
	1    3050 1800
	1    0    0    -1  
$EndComp
Wire Wire Line
	1700 1450 2850 1450
Wire Wire Line
	3050 1450 3050 1650
Wire Wire Line
	2850 1950 2850 2400
Wire Wire Line
	2850 1650 2850 1450
Connection ~ 2850 1450
Wire Wire Line
	2850 1450 3050 1450
Wire Wire Line
	3050 1950 3050 2200
Wire Wire Line
	3100 2200 3050 2200
Connection ~ 3050 2200
Wire Wire Line
	2900 2400 2850 2400
Connection ~ 2850 2400
Text GLabel 4000 2100 0    50   Input ~ 0
F_OUT
Wire Wire Line
	5550 2600 5550 2700
Connection ~ 5550 2600
Wire Wire Line
	5800 2600 5550 2600
Wire Wire Line
	5800 2100 5550 2100
Text GLabel 5800 2600 2    50   Input ~ 0
M0
Wire Wire Line
	5550 2550 5550 2600
Text GLabel 5800 2100 2    50   Input ~ 0
M1
$Comp
L power:GND #PWR0102
U 1 1 5EEF678E
P 5550 2700
F 0 "#PWR0102" H 5550 2450 50  0001 C CNN
F 1 "GND" H 5555 2527 50  0000 C CNN
F 2 "" H 5550 2700 50  0001 C CNN
F 3 "" H 5550 2700 50  0001 C CNN
	1    5550 2700
	1    0    0    -1  
$EndComp
$Comp
L Device:C C4
U 1 1 5EEC01B1
P 5550 2400
F 0 "C4" H 5665 2446 50  0000 L CNN
F 1 "47pF" H 5665 2355 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P10.16mm_Horizontal" H 5588 2250 50  0001 C CNN
F 3 "~" H 5550 2400 50  0001 C CNN
	1    5550 2400
	1    0    0    -1  
$EndComp
Wire Wire Line
	5550 2250 5550 2100
$Comp
L Device:R R3
U 1 1 5EEBFBC4
P 4750 1600
F 0 "R3" V 4543 1600 50  0000 C CNN
F 1 "R" V 4634 1600 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P10.16mm_Horizontal" V 4680 1600 50  0001 C CNN
F 3 "~" H 4750 1600 50  0001 C CNN
	1    4750 1600
	0    1    1    0   
$EndComp
Wire Wire Line
	4050 2100 4450 2100
Wire Wire Line
	4050 1600 4600 1600
Wire Wire Line
	4900 1600 5250 1600
Wire Wire Line
	5250 1600 5250 2100
Wire Wire Line
	5250 2100 5050 2100
Wire Wire Line
	4050 2100 4050 1600
Wire Notes Line
	3400 1100 3400 3700
Wire Wire Line
	4000 2100 4050 2100
Connection ~ 4050 2100
$Comp
L Device:C C3
U 1 1 5EF777BE
P 2700 4600
F 0 "C3" H 2815 4646 50  0000 L CNN
F 1 "C" H 2815 4555 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P10.16mm_Horizontal" H 2738 4450 50  0001 C CNN
F 3 "~" H 2700 4600 50  0001 C CNN
	1    2700 4600
	1    0    0    -1  
$EndComp
Text GLabel 2550 2700 2    50   Input ~ 0
S2
$Comp
L Device:Thermistor_NTC TH1
U 1 1 5EF7D11D
P 2500 4550
F 0 "TH1" V 2500 4450 50  0000 L CNN
F 1 "Thermistor_NTC" V 2300 4200 50  0000 L CNN
F 2 "Capacitor_THT:C_Disc_D5.0mm_W2.5mm_P2.50mm" H 2500 4600 50  0001 C CNN
F 3 "~" H 2500 4600 50  0001 C CNN
	1    2500 4550
	1    0    0    -1  
$EndComp
$Comp
L Device:R R4
U 1 1 5EF99FBB
P 6500 5100
F 0 "R4" V 6500 5000 50  0000 L CNN
F 1 "R" V 6600 5050 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P10.16mm_Horizontal" V 6430 5100 50  0001 C CNN
F 3 "~" H 6500 5100 50  0001 C CNN
	1    6500 5100
	1    0    0    -1  
$EndComp
Text GLabel 9650 2400 3    50   Input ~ 0
NTC_H
Wire Wire Line
	2500 4300 2500 4400
Text GLabel 6500 5250 3    50   Input ~ 0
NTC_L
Text GLabel 2700 4250 1    50   Input ~ 0
CR_H
Wire Wire Line
	2700 4250 2700 4450
Text GLabel 2700 5050 3    50   Input ~ 0
CR_L
Wire Wire Line
	2700 4750 2700 5050
Wire Wire Line
	8200 3600 8600 3600
Wire Wire Line
	8200 1950 8600 1950
Text GLabel 8600 1950 2    50   Input ~ 0
CR_H
Text GLabel 8600 3600 2    50   Input ~ 0
CR_L
$Comp
L 74xx:74HC4051 U4
U 1 1 5EED1058
P 7800 1850
F 0 "U4" H 7300 2500 50  0000 C CNN
F 1 "74HC4051" H 7450 2400 50  0000 C CNN
F 2 "Package_DIP:DIP-16_W7.62mm" H 7800 1450 50  0001 C CNN
F 3 "http://www.ti.com/lit/ds/symlink/cd74hc4051.pdf" H 7800 1450 50  0001 C CNN
	1    7800 1850
	1    0    0    -1  
$EndComp
$Comp
L 74xx:74HC4051 U5
U 1 1 5EED14DE
P 7800 3500
F 0 "U5" H 7350 4150 50  0000 C CNN
F 1 "74HC4051" H 7500 4050 50  0000 C CNN
F 2 "Package_DIP:DIP-16_W7.62mm" H 7800 3100 50  0001 C CNN
F 3 "http://www.ti.com/lit/ds/symlink/cd74hc4051.pdf" H 7800 3100 50  0001 C CNN
	1    7800 3500
	1    0    0    -1  
$EndComp
Wire Wire Line
	6950 2150 7500 2150
Wire Wire Line
	6950 3800 7500 3800
Wire Wire Line
	6950 2150 6950 3800
Wire Wire Line
	7050 3600 7500 3600
Wire Wire Line
	7150 3500 7500 3500
Wire Wire Line
	7150 1850 7150 3500
Wire Wire Line
	7500 1850 7150 1850
Wire Wire Line
	7250 3400 7500 3400
Wire Wire Line
	7250 1750 7250 3400
Wire Wire Line
	7500 1750 7250 1750
Text GLabel 7500 3200 0    50   Input ~ 0
M0
Text GLabel 7500 1550 0    50   Input ~ 0
M1
Wire Wire Line
	7900 5800 7900 5900
Wire Wire Line
	7250 5100 7500 5100
Wire Wire Line
	7500 5500 6950 5500
Text GLabel 6300 4900 0    50   Input ~ 0
A2
Text GLabel 2500 4300 1    50   Input ~ 0
NTC_H
Text GLabel 2500 5150 3    50   Input ~ 0
TR
Text GLabel 8750 5300 2    50   Input ~ 0
TR
Wire Wire Line
	8200 5300 8750 5300
Wire Wire Line
	2500 5150 2500 4700
Wire Wire Line
	7800 5800 7800 5900
$Comp
L power:GND #PWR0113
U 1 1 5F06DB7B
P 7800 5900
F 0 "#PWR0113" H 7800 5650 50  0001 C CNN
F 1 "GND" H 7805 5727 50  0000 C CNN
F 2 "" H 7800 5900 50  0001 C CNN
F 3 "" H 7800 5900 50  0001 C CNN
	1    7800 5900
	1    0    0    -1  
$EndComp
Text Notes 4550 1050 0    50   ~ 0
Schmitt Trigger
Text Notes 2100 1050 0    50   ~ 0
ATTINY
Wire Notes Line
	6150 1100 10400 1100
Wire Notes Line
	9000 1100 9000 6200
Wire Notes Line
	6150 6200 6150 1100
Text Notes 7500 1050 0    50   ~ 0
Multiplexer, cap-IN, cap-GND, ntc-IN
Wire Notes Line
	2150 3900 2150 5500
Wire Notes Line
	2150 5500 3100 5500
Wire Notes Line
	3100 5500 3100 3900
Wire Notes Line
	3100 3900 2150 3900
Text Notes 2450 3850 0    50   ~ 0
Reference\n
Wire Notes Line
	900  3900 2000 3900
Wire Notes Line
	2000 3900 2000 4900
Wire Notes Line
	2000 4900 900  4900
Wire Notes Line
	900  4900 900  3900
Text Notes 1250 3850 0    50   ~ 0
Decoupling
Text Notes 1400 5100 0    50   ~ 0
I2C
$Comp
L power:+3.3V #PWR0116
U 1 1 5EED1BA9
P 4550 6500
F 0 "#PWR0116" H 4550 6350 50  0001 C CNN
F 1 "+3.3V" V 4565 6628 50  0000 L CNN
F 2 "" H 4550 6500 50  0001 C CNN
F 3 "" H 4550 6500 50  0001 C CNN
	1    4550 6500
	0    -1   -1   0   
$EndComp
$Comp
L power:GND #PWR0118
U 1 1 5EED3513
P 4850 6200
F 0 "#PWR0118" H 4850 5950 50  0001 C CNN
F 1 "GND" H 4855 6027 50  0000 C CNN
F 2 "" H 4850 6200 50  0001 C CNN
F 3 "" H 4850 6200 50  0001 C CNN
	1    4850 6200
	-1   0    0    1   
$EndComp
Wire Notes Line
	3700 5900 6000 5900
Wire Notes Line
	6000 5900 6000 7000
Wire Notes Line
	6000 7000 3700 7000
Wire Notes Line
	3700 7000 3700 5900
$Comp
L 74xx:74HC14 U2
U 1 1 5EEFFE76
P 4750 2100
F 0 "U2" H 4750 1783 50  0000 C CNN
F 1 "74HC14" H 4750 1874 50  0000 C CNN
F 2 "Package_DIP:DIP-14_W7.62mm" H 4750 2100 50  0001 C CNN
F 3 "http://www.ti.com/lit/gpn/sn74HC14" H 4750 2100 50  0001 C CNN
	1    4750 2100
	-1   0    0    1   
$EndComp
$Comp
L 74xx:74HC14 U2
U 2 1 5EF0545F
P 4750 2600
F 0 "U2" H 4750 2283 50  0000 C CNN
F 1 "74HC14" H 4750 2374 50  0000 C CNN
F 2 "Package_DIP:DIP-14_W7.62mm" H 4750 2600 50  0001 C CNN
F 3 "http://www.ti.com/lit/gpn/sn74HC14" H 4750 2600 50  0001 C CNN
	2    4750 2600
	-1   0    0    1   
$EndComp
$Comp
L 74xx:74HC14 U2
U 3 1 5EF073CB
P 4750 3100
F 0 "U2" H 4750 2783 50  0000 C CNN
F 1 "74HC14" H 4750 2874 50  0000 C CNN
F 2 "Package_DIP:DIP-14_W7.62mm" H 4750 3100 50  0001 C CNN
F 3 "http://www.ti.com/lit/gpn/sn74HC14" H 4750 3100 50  0001 C CNN
	3    4750 3100
	-1   0    0    1   
$EndComp
$Comp
L 74xx:74HC14 U2
U 5 1 5EF09F78
P 4750 4100
F 0 "U2" H 4750 3783 50  0000 C CNN
F 1 "74HC14" H 4750 3874 50  0000 C CNN
F 2 "Package_DIP:DIP-14_W7.62mm" H 4750 4100 50  0001 C CNN
F 3 "http://www.ti.com/lit/gpn/sn74HC14" H 4750 4100 50  0001 C CNN
	5    4750 4100
	-1   0    0    1   
$EndComp
$Comp
L 74xx:74HC14 U2
U 6 1 5EF1A7E4
P 4750 4650
F 0 "U2" H 4750 4333 50  0000 C CNN
F 1 "74HC14" H 4750 4424 50  0000 C CNN
F 2 "Package_DIP:DIP-14_W7.62mm" H 4750 4650 50  0001 C CNN
F 3 "http://www.ti.com/lit/gpn/sn74HC14" H 4750 4650 50  0001 C CNN
	6    4750 4650
	-1   0    0    1   
$EndComp
$Comp
L 74xx:74HC14 U2
U 7 1 5EF1CAF5
P 4750 5300
F 0 "U2" V 4383 5300 50  0000 C CNN
F 1 "74HC14" V 4474 5300 50  0000 C CNN
F 2 "Package_DIP:DIP-14_W7.62mm" H 4750 5300 50  0001 C CNN
F 3 "http://www.ti.com/lit/gpn/sn74HC14" H 4750 5300 50  0001 C CNN
	7    4750 5300
	0    1    1    0   
$EndComp
Wire Wire Line
	5050 2600 5050 3100
Wire Wire Line
	5050 3600 5050 4100
Connection ~ 5050 4100
Wire Wire Line
	5050 4100 5050 4650
$Comp
L power:GND #PWR0119
U 1 1 5EF6D9C4
P 4250 5300
F 0 "#PWR0119" H 4250 5050 50  0001 C CNN
F 1 "GND" V 4255 5172 50  0000 R CNN
F 2 "" H 4250 5300 50  0001 C CNN
F 3 "" H 4250 5300 50  0001 C CNN
	1    4250 5300
	0    1    1    0   
$EndComp
$Comp
L power:+2V5 #PWR0120
U 1 1 5EF706F5
P 5250 5300
F 0 "#PWR0120" H 5250 5150 50  0001 C CNN
F 1 "+2V5" V 5265 5428 50  0000 L CNN
F 2 "" H 5250 5300 50  0001 C CNN
F 3 "" H 5250 5300 50  0001 C CNN
	1    5250 5300
	0    1    1    0   
$EndComp
Wire Notes Line
	3650 1100 3650 5750
Wire Notes Line
	3650 5750 6000 5750
Wire Notes Line
	6000 5750 6000 1100
Wire Notes Line
	3650 1100 6000 1100
Connection ~ 7250 3400
Connection ~ 7150 3500
Connection ~ 6950 3800
Wire Wire Line
	7800 2450 7850 2450
Wire Wire Line
	7800 4100 7850 4100
Wire Wire Line
	7500 5200 7150 5200
Wire Wire Line
	7050 5300 7500 5300
$Comp
L 74xx:74HC4051 U6
U 1 1 5F032EE0
P 7800 5200
F 0 "U6" H 7300 5800 50  0000 C CNN
F 1 "74HC4051" H 7450 5700 50  0000 C CNN
F 2 "Package_DIP:DIP-16_W7.62mm" H 7800 4800 50  0001 C CNN
F 3 "http://www.ti.com/lit/ds/symlink/cd74hc4051.pdf" H 7800 4800 50  0001 C CNN
	1    7800 5200
	1    0    0    -1  
$EndComp
Connection ~ 7800 5900
Connection ~ 7050 3600
Wire Wire Line
	7500 1950 7050 1950
Wire Wire Line
	7050 1950 7050 3600
Text GLabel 6850 5300 0    50   Input ~ 0
S2
Text GLabel 6850 5200 0    50   Input ~ 0
S1
Text GLabel 6850 5100 0    50   Input ~ 0
S0
Wire Wire Line
	6300 4900 6500 4900
Wire Wire Line
	6500 4950 6500 4900
Connection ~ 6500 4900
Wire Wire Line
	6500 4900 7500 4900
Wire Wire Line
	6850 5100 7250 5100
Connection ~ 7250 5100
Wire Wire Line
	6850 5200 7150 5200
Connection ~ 7150 5200
Wire Wire Line
	6850 5300 7050 5300
Connection ~ 7050 5300
Wire Wire Line
	7800 5900 7900 5900
Wire Wire Line
	7850 4100 7850 4150
Connection ~ 7850 4100
Wire Wire Line
	7850 4100 7900 4100
Wire Wire Line
	7850 2500 7850 2450
Connection ~ 7850 2450
Wire Wire Line
	7850 2450 7900 2450
$Comp
L Connector:Conn_01x02_Female J5
U 1 1 5F4655DC
P 1400 6150
F 0 "J5" H 1428 6126 50  0000 L CNN
F 1 "Conn_01x02_Female" H 1428 6035 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x02_P2.54mm_Vertical" H 1400 6150 50  0001 C CNN
F 3 "~" H 1400 6150 50  0001 C CNN
	1    1400 6150
	1    0    0    -1  
$EndComp
Text GLabel 2550 2300 2    50   Input ~ 0
MISO
Text GLabel 2550 3000 2    50   Input ~ 0
RESET
Text GLabel 1200 6150 0    50   Input ~ 0
MISO
Text GLabel 1200 6250 0    50   Input ~ 0
RESET
Wire Wire Line
	6950 3800 6950 5500
Wire Wire Line
	7050 3600 7050 5300
Wire Wire Line
	7150 3500 7150 5200
Wire Wire Line
	7250 3400 7250 5100
$Comp
L power:GND #PWR0103
U 1 1 5F47BF89
P 7850 2500
F 0 "#PWR0103" H 7850 2250 50  0001 C CNN
F 1 "GND" H 7855 2327 50  0000 C CNN
F 2 "" H 7850 2500 50  0001 C CNN
F 3 "" H 7850 2500 50  0001 C CNN
	1    7850 2500
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0104
U 1 1 5F47C99B
P 7850 4150
F 0 "#PWR0104" H 7850 3900 50  0001 C CNN
F 1 "GND" H 7855 3977 50  0000 C CNN
F 2 "" H 7850 4150 50  0001 C CNN
F 3 "" H 7850 4150 50  0001 C CNN
	1    7850 4150
	1    0    0    -1  
$EndComp
$Comp
L power:+2V5 #PWR0117
U 1 1 5EED2B14
P 5350 6500
F 0 "#PWR0117" H 5350 6350 50  0001 C CNN
F 1 "+2V5" V 5365 6628 50  0000 L CNN
F 2 "" H 5350 6500 50  0001 C CNN
F 3 "" H 5350 6500 50  0001 C CNN
	1    5350 6500
	0    1    1    0   
$EndComp
$Comp
L Regulator_Linear:MCP1700-2502E_TO92 U3
U 1 1 5EECDB80
P 4850 6500
F 0 "U3" H 4850 6351 50  0000 C CNN
F 1 "MCP1700-2502E_TO92" H 4850 6260 50  0000 C CNN
F 2 "Package_TO_SOT_THT:TO-92_Inline_Wide" H 4850 6300 50  0001 C CIN
F 3 "http://ww1.microchip.com/downloads/en/DeviceDoc/20001826C.pdf" H 4850 6500 50  0001 C CNN
	1    4850 6500
	1    0    0    -1  
$EndComp
Text GLabel 2550 2100 2    50   Input ~ 0
A3
Connection ~ 1700 1450
Wire Wire Line
	1700 1450 1700 1400
$Comp
L power:GND #PWR0124
U 1 1 5FC7530F
P 8200 3900
F 0 "#PWR0124" H 8200 3650 50  0001 C CNN
F 1 "GND" V 8205 3772 50  0000 R CNN
F 2 "" H 8200 3900 50  0001 C CNN
F 3 "" H 8200 3900 50  0001 C CNN
	1    8200 3900
	0    -1   -1   0   
$EndComp
Wire Notes Line
	6150 6200 10400 6200
Text GLabel 8200 1550 2    50   Input ~ 0
C1_A0
Text GLabel 8200 1650 2    50   Input ~ 0
C1_A1
Text GLabel 8200 1750 2    50   Input ~ 0
C1_A2
Text GLabel 8200 1850 2    50   Input ~ 0
C1_A3
Text GLabel 8200 3200 2    50   Input ~ 0
C0_A0
Text GLabel 8200 3300 2    50   Input ~ 0
C0_A1
Text GLabel 8200 3400 2    50   Input ~ 0
C0_A2
Text GLabel 8200 3500 2    50   Input ~ 0
C0_A3
Text GLabel 8200 4900 2    50   Input ~ 0
NTC_A0
Text GLabel 8200 5000 2    50   Input ~ 0
NTC_A1
Text GLabel 8200 5100 2    50   Input ~ 0
NTC_A2
Text GLabel 8200 5200 2    50   Input ~ 0
NTC_A3
$Comp
L Connector:Conn_01x08_Female J4
U 1 1 6044449B
P 9650 1550
F 0 "J4" H 9678 1526 50  0000 L CNN
F 1 "Conn_01x08_Female" H 9678 1435 50  0000 L CNN
F 2 "Connector_PinHeader_1.00mm:PinHeader_1x08_P1.00mm_Vertical" H 9650 1550 50  0001 C CNN
F 3 "~" H 9650 1550 50  0001 C CNN
	1    9650 1550
	0    -1   -1   0   
$EndComp
$Comp
L Connector:Conn_01x08_Female J3
U 1 1 60445292
P 9550 2200
F 0 "J3" H 9578 2176 50  0000 L CNN
F 1 "Conn_01x08_Female" H 9578 2085 50  0000 L CNN
F 2 "Connector_PinHeader_1.00mm:PinHeader_1x08_P1.00mm_Vertical" H 9550 2200 50  0001 C CNN
F 3 "~" H 9550 2200 50  0001 C CNN
	1    9550 2200
	0    -1   -1   0   
$EndComp
Text GLabel 9950 2400 3    50   Input ~ 0
C1_A0
Text GLabel 10050 1750 3    50   Input ~ 0
C1_A1
Text GLabel 9950 1750 3    50   Input ~ 0
C1_A2
Text GLabel 9850 2400 3    50   Input ~ 0
C1_A3
Text GLabel 9450 1750 3    50   Input ~ 0
C0_A3
Text GLabel 9350 1750 3    50   Input ~ 0
C0_A0
Text GLabel 9250 2400 3    50   Input ~ 0
C0_A1
Text GLabel 9350 2400 3    50   Input ~ 0
C0_A2
Text GLabel 9550 2400 3    50   Input ~ 0
NTC_A0
Text GLabel 9650 1750 3    50   Input ~ 0
NTC_A1
Text GLabel 9550 1750 3    50   Input ~ 0
NTC_A2
Text GLabel 9450 2400 3    50   Input ~ 0
NTC_A3
Text GLabel 9750 2400 3    50   Input ~ 0
NTC_H
Text GLabel 9750 1750 3    50   Input ~ 0
NTC_H
Text GLabel 9850 1750 3    50   Input ~ 0
NTC_H
$Comp
L power:+2V5 #PWR0101
U 1 1 604FDBF4
P 1700 1400
F 0 "#PWR0101" H 1700 1250 50  0001 C CNN
F 1 "+2V5" H 1715 1573 50  0000 C CNN
F 2 "" H 1700 1400 50  0001 C CNN
F 3 "" H 1700 1400 50  0001 C CNN
	1    1700 1400
	1    0    0    -1  
$EndComp
$Comp
L power:+2V5 #PWR0105
U 1 1 6050EE77
P 1200 4250
F 0 "#PWR0105" H 1200 4100 50  0001 C CNN
F 1 "+2V5" H 1215 4423 50  0000 C CNN
F 2 "" H 1200 4250 50  0001 C CNN
F 3 "" H 1200 4250 50  0001 C CNN
	1    1200 4250
	1    0    0    -1  
$EndComp
Connection ~ 1200 4250
$Comp
L power:+2V5 #PWR0109
U 1 1 60530E4F
P 8200 2250
F 0 "#PWR0109" H 8200 2100 50  0001 C CNN
F 1 "+2V5" V 8215 2378 50  0000 L CNN
F 2 "" H 8200 2250 50  0001 C CNN
F 3 "" H 8200 2250 50  0001 C CNN
	1    8200 2250
	0    1    1    0   
$EndComp
$Comp
L power:+2V5 #PWR0110
U 1 1 60531C55
P 7800 1350
F 0 "#PWR0110" H 7800 1200 50  0001 C CNN
F 1 "+2V5" H 7815 1523 50  0000 C CNN
F 2 "" H 7800 1350 50  0001 C CNN
F 3 "" H 7800 1350 50  0001 C CNN
	1    7800 1350
	1    0    0    -1  
$EndComp
$Comp
L power:+2V5 #PWR0112
U 1 1 60543996
P 7800 3000
F 0 "#PWR0112" H 7800 2850 50  0001 C CNN
F 1 "+2V5" H 7815 3173 50  0000 C CNN
F 2 "" H 7800 3000 50  0001 C CNN
F 3 "" H 7800 3000 50  0001 C CNN
	1    7800 3000
	1    0    0    -1  
$EndComp
$Comp
L power:+2V5 #PWR0114
U 1 1 60544691
P 7800 4700
F 0 "#PWR0114" H 7800 4550 50  0001 C CNN
F 1 "+2V5" H 7815 4873 50  0000 C CNN
F 2 "" H 7800 4700 50  0001 C CNN
F 3 "" H 7800 4700 50  0001 C CNN
	1    7800 4700
	1    0    0    -1  
$EndComp
$Comp
L 74xx:74HC14 U2
U 4 1 5EF108CD
P 4750 3600
F 0 "U2" H 4750 3283 50  0000 C CNN
F 1 "74HC14" H 4750 3374 50  0000 C CNN
F 2 "Package_DIP:DIP-14_W7.62mm" H 4750 3600 50  0001 C CNN
F 3 "http://www.ti.com/lit/gpn/sn74HC14" H 4750 3600 50  0001 C CNN
	4    4750 3600
	-1   0    0    1   
$EndComp
$Comp
L power:GND #PWR0115
U 1 1 60559FAD
P 5050 3200
F 0 "#PWR0115" H 5050 2950 50  0001 C CNN
F 1 "GND" H 5055 3027 50  0000 C CNN
F 2 "" H 5050 3200 50  0001 C CNN
F 3 "" H 5050 3200 50  0001 C CNN
	1    5050 3200
	1    0    0    -1  
$EndComp
Wire Wire Line
	5050 3100 5050 3200
Connection ~ 5050 3100
$Comp
L power:+2V5 #PWR0121
U 1 1 60566F9C
P 5050 4650
F 0 "#PWR0121" H 5050 4500 50  0001 C CNN
F 1 "+2V5" H 5065 4823 50  0000 C CNN
F 2 "" H 5050 4650 50  0001 C CNN
F 3 "" H 5050 4650 50  0001 C CNN
	1    5050 4650
	-1   0    0    1   
$EndComp
Connection ~ 5050 4650
Wire Wire Line
	5150 6500 5350 6500
$Comp
L power:GND #PWR0122
U 1 1 605934F3
P 6950 5500
F 0 "#PWR0122" H 6950 5250 50  0001 C CNN
F 1 "GND" H 6955 5327 50  0000 C CNN
F 2 "" H 6950 5500 50  0001 C CNN
F 3 "" H 6950 5500 50  0001 C CNN
	1    6950 5500
	1    0    0    -1  
$EndComp
Connection ~ 6950 5500
Wire Wire Line
	2300 3000 2550 3000
Wire Wire Line
	2600 2900 2300 2900
Wire Wire Line
	2300 2800 2550 2800
Wire Wire Line
	2300 2700 2550 2700
Wire Wire Line
	2300 2500 2550 2500
Wire Wire Line
	2300 2400 2850 2400
Wire Wire Line
	2550 2300 2300 2300
Wire Wire Line
	2300 2200 3050 2200
Wire Wire Line
	2300 2100 2550 2100
Wire Wire Line
	2300 2000 2550 2000
Wire Wire Line
	2300 1900 2550 1900
Wire Wire Line
	2300 1800 2550 1800
$Comp
L MCU_Microchip_ATtiny:ATtiny84-20PU U1
U 1 1 5EDB8D88
P 1700 2400
F 0 "U1" H 1170 2446 50  0000 R CNN
F 1 "ATtiny84-20PU" V 1100 3200 50  0000 R CNN
F 2 "Package_DIP:DIP-14_W7.62mm" H 1700 2400 50  0001 C CIN
F 3 "http://ww1.microchip.com/downloads/en/DeviceDoc/doc8006.pdf" H 1700 2400 50  0001 C CNN
	1    1700 2400
	1    0    0    -1  
$EndComp
Wire Wire Line
	1700 3300 1700 3400
$Comp
L Device:R R?
U 1 1 605B91A3
P 5400 2100
F 0 "R?" V 5193 2100 50  0000 C CNN
F 1 "R" V 5284 2100 50  0000 C CNN
F 2 "" V 5330 2100 50  0001 C CNN
F 3 "~" H 5400 2100 50  0001 C CNN
	1    5400 2100
	0    1    1    0   
$EndComp
Connection ~ 5250 2100
Connection ~ 5550 2100
$EndSCHEMATC
